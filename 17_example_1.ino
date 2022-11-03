#include <Servo.h>

// Arduino pin assignment

#define PIN_POTENTIOMETER 3 // Potentiometer at Pin A3
#define PIN_IR 0
#define PIN_LED 9
#define PIN_SERVO 10

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)

#define DIST_MIN 100.0
#define DIST_MAX 250.0

#define _EMA_ALPHA 0.1

#define LOOP_INTERVAL 20   // Loop Interval (unit: msec)

Servo myservo;
unsigned long last_loop_time;   // unit: msec
float dist_ema, dist_prev = 0;

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  Serial.begin(2000000);
}

void loop()
{
  unsigned long time_curr = millis();
  int a_value, duty;
  float dist;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  // Remove Next line !!!
//  a_value = analogRead(PIN_POTENTIOMETER);
  // Read IR Sensor value !!!
  a_value = analogRead(PIN_IR);
  // Convert IR sensor value into distance !!!
  dist = (6762.0/(a_value-9)-4.0)*10.0 - 60.0;
  // we need distance range filter here !!!
  if (dist >= DIST_MIN && dist <= DIST_MAX) {
    digitalWrite(PIN_LED, 0);
    dist_prev = dist;
  } else {
    digitalWrite(PIN_LED, 1);
    dist = dist_prev;
  }
  // we need EMA filter here !!!
  dist_ema = (_EMA_ALPHA*dist) + ((1-_EMA_ALPHA)*dist_ema);
  // map distance into duty
  duty = _DUTY_MIN + ((dist_ema - 100.0) / 150.0) * (_DUTY_MAX - _DUTY_MIN);
//  duty = map(a_value, 0, 1023, _DUTY_MIN, _DUTY_MAX);
  myservo.writeMicroseconds(duty);

  // print IR sensor value, distnace, duty !!!
  Serial.print("MIN:"); Serial.print(DIST_MIN);
  Serial.print(",IR:"); Serial.print(a_value);
  Serial.print(",dist:"); Serial.print(dist);
  Serial.print(",ema:"); Serial.print(dist_ema);
  Serial.print(",servo:"); Serial.print(duty);
  Serial.print(",MAX:"); Serial.print(DIST_MAX);
  Serial.println("");
//  Serial.print("ADC Read: "); Serial.print(a_value);
//  Serial.print(" = ");
//  Serial.print((a_value / 1024.0) * 5.0);
//  Serial.print(" Volt => Duty : ");
//  Serial.print(duty);
//  Serial.println("usec");
}
