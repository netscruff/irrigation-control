#include "Timer.h"
// constants won't change. They're used here to
// wiring configureation
// Wireless receiver -> Arduino
// D0 -> D3
// D1 -> D4
// D2 -> D5
// D3 -> D6

// Relay module
// IN1 -> D9
// IN2 -> D10
// set pin numbers:
const int APin = 5;
const int BPin = 3;
const int CPin = 6;
const int DPin = 4;
const int R1 = 9;
const int R2 = 10;
const int LED = 13;
int aState = 0;
int bState = 0;
int cState = 0;
int dState = 0;
int lastButtonPressed = 0;
int currentEvent = 0;
int afterEvent = 0;
unsigned long period;
Timer t;
 
void setup() {
  // initialize D2 as 5V for receiver
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  // initialize the pushbutton pin as an input:
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(13, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(13, LOW);
  int tickEvent = t.every(100, check_switches, (void*)0);
  Serial.begin(57600);
}
 
void loop() {
  t.update();
}

void check_switches(void *context)
{
  aState = digitalRead(APin);
  bState = digitalRead(BPin);
  cState = digitalRead(CPin);
  dState = digitalRead(DPin);

  if (aState == HIGH) {
    Serial.println("A Pressed");
    if (lastButtonPressed != APin)
    {
      period = 60 * 1000;
      pump_on(period);
      lastButtonPressed = APin;
    }
  }
  if (bState == HIGH) {
    Serial.println("B Pressed");
    if (lastButtonPressed != BPin)
    {
      period = 5 * 60 * 1000;
      pump_on(period);
      lastButtonPressed = BPin;
    }
  }
  if (cState == HIGH) {
    Serial.println("C Pressed");
    if (lastButtonPressed != CPin)
    {
      period = 15 * 60 * 1000;
      pump_on(period);
      lastButtonPressed = CPin;
    }
  }
  if (dState == HIGH) {
    Serial.println("D Pressed");
    lastButtonPressed = DPin;
    pump_off();
  }
}

void pump_on(unsigned long period)
{
  currentEvent = t.pulseImmediate(R1, period, LOW);
  currentEvent = t.pulseImmediate(LED, period, HIGH);
  //afterEvent = t.after(period + 10, reset_button_pressed, (void*)0);
}

void reset_button_pressed(void *context)
{
  lastButtonPressed = 0;
}

void pump_off()
{
  Serial.println("Turning pump off");
  digitalWrite(R1, HIGH);
  digitalWrite(LED, LOW);
  if (currentEvent != 0) {
    t.stop(currentEvent);
    currentEvent = 0;
    t.stop(afterEvent);
    afterEvent = 0;
  }
  reset_button_pressed((void*)0);
}
