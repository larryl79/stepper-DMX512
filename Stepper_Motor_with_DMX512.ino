
#include <DMXSerial.h>

#define MAX_ANGLE 360
#define STEPS_FOR360 400

const byte LED13 =   13;
const byte LED02 =   2;

const byte DIP0 = 3;
const byte DIP1 = 4;
const byte DIP2 = 5;
const byte DIP3 = 6;
const byte DIP4 = 7;
const byte DIP5 = 8;
const byte DIP6 = 9;
const byte DIP7 = 10;
const byte DIP8 = 11;
//const byte DIP9 = 12;
int channelNumber = 0;

const byte stepPin = 16;
const byte dirPin =15;
const byte enPin = 14;
int StepperCurrentAngle = 0;
int currentStepsfrom0 = 0;
float seekingAngle;
float stepsForSeekingAngle;

int DMXvalue = 0;

void setup() {

  DMXSerial.init(DMXReceiver);

  DMXSerial.maxChannel(192);

  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,HIGH); // to make Stepper motor enable
  pinMode(LED02,OUTPUT);

  pinMode(DIP0,INPUT_PULLUP);
  pinMode(DIP1,INPUT_PULLUP);
  pinMode(DIP2,INPUT_PULLUP);
  pinMode(DIP3,INPUT_PULLUP);
  pinMode(DIP4,INPUT_PULLUP);
  pinMode(DIP5,INPUT_PULLUP);
  pinMode(DIP6,INPUT_PULLUP);
  pinMode(DIP7,INPUT_PULLUP);
  pinMode(DIP8,INPUT_PULLUP);
//  pinMode(DIP9,INPUT_PULLUP);
//  channelNumber = (1 * !digitalRead(DIP0)) + (2 * !digitalRead(DIP1)) + (4 * !digitalRead(DIP2)) + (8 * !digitalRead(DIP3)) + (16 * !digitalRead(DIP4)) + (32 * !digitalRead(DIP5)) + (64 * !digitalRead(DIP6)) + (128 * !digitalRead(DIP7)) + (256 * !digitalRead(DIP8)) + (512 * !digitalRead(DIP9));
  channelNumber = (1 * !digitalRead(DIP0)) + (2 * !digitalRead(DIP1)) + (4 * !digitalRead(DIP2)) + (8 * !digitalRead(DIP3)) + (16 * !digitalRead(DIP4)) + (32 * !digitalRead(DIP5)) + (64 * !digitalRead(DIP6)) + (128 * !digitalRead(DIP7)) + (256 * !digitalRead(DIP8));
}


void loop() {
  // Calculate how long no data backet was received
  unsigned long lastPacket = DMXSerial.noDataSince();

  if (lastPacket < 5000) {
    DMXvalue = DMXSerial.read(channelNumber);
    if(DMXvalue > 125) digitalWrite(LED02,HIGH); else digitalWrite(LED02,LOW); 
    seekingAngle = map(DMXvalue,0,255,0,MAX_ANGLE);
    stepsForSeekingAngle = (float)STEPS_FOR360 * seekingAngle / 360;
    rotate(stepsForSeekingAngle - currentStepsfrom0);
    digitalWrite(LED13,LOW); 
  } else {
    digitalWrite(LED13,HIGH); 
  } // if
}

void rotate(int steps)
{
    if(steps>0)
    {
      digitalWrite(dirPin,LOW);    // clockwise driection
      for(int x = 0; x < steps; x++) {
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(1000);
        digitalWrite(stepPin,LOW);
        delayMicroseconds(1000);
        currentStepsfrom0++;
      }
      delayMicroseconds(10000);
    }
    else
    {  
      digitalWrite(dirPin,HIGH);     //  anticlockwise direction
      for(int x = steps; x < 0 ; x++) {
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(1000);
        digitalWrite(stepPin,LOW);
        delayMicroseconds(1000);
        currentStepsfrom0--;
      }
      delayMicroseconds(10000);
    } 
}
