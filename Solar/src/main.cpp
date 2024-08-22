#include <Arduino.h>
#include<RTClib.h>
#include<SPI.h>
#include<ESP32Servo.h>

// put function declarations here:
const int LDR1_PIN =34;
const int LDR2_PIN =35;
const int POT_PIN = 32;
const int SERVO_PIN =15;

RTC_DS3231 rtc;
Servo solarServo;
int ldr1value=0;
int ldr2value=0;
int potvalue=0;
int threshold=0;
void adjustPanel();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if(!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
  }
  if(rtc.lostPower())
  {
    Serial.println("RTC lost power ...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  solarServo.attach(SERVO_PIN);
  solarServo.write(0);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  ldr1value=analogRead(LDR1_PIN);
  ldr2value=analogRead(LDR2_PIN);

  potvalue = analogRead(POT_PIN);
  threshold =map(potvalue,0,4095,0,1023);

  DateTime now = rtc.now();
  int currentHour = now.hour();

  if(currentHour >=6 && currentHour<12)
  {
    adjustPanel();
  }
  else if(currentHour>=12 && currentHour <18)
  {
    adjustPanel();
  }
  else if(currentHour>=18 || currentHour< 6)
  {
    adjustPanel();
  }
  else{
    solarServo.write(0);
  }
  delay(1000);
}

void adjustPanel()
{
  int newAngle;
  int currentAngle= solarServo.read();
  if(ldr1value >ldr2value + threshold)
  {
    newAngle = currentAngle-1;
    if(newAngle >=0)
    {
      solarServo.write(newAngle);
    }
  }
  else if(ldr2value >ldr1value + threshold)
  {
    newAngle = currentAngle+1;
    if(newAngle<= 180)
    {
      solarServo.write(newAngle);
    }
  }
}


