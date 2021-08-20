
#include <Servo.h>


//INPUT sensor sharp
int ir[4] = {A0, A1, A2, A3};
//INPUT Receiver
#define aeleronIN  A4
#define elevatorIN  A5
#define modeIN  2
#define triggerIN  3
#define inmode  9
//OUTPUT Receiver
#define aeleronOUT 11
#define elevatorOUT 12
#define modeOUT 13

//ServoINPUT
#define servofree 4
//ServoOUTPUT
#define outservocam 5
#define outservoarm 6
#define outservofree 7
//optional tambahan
#define lampu1 8
#define buzzer 10


int left, front, right, back;//nilai sensor
int temp = 0, temp1 = 0, temp2 = 0, temp3 = 0; //nilai Real
int e1, e2, e3, e4;//perbandingan nilai Real dengan Setpoint
int op1, op2, op3, op4, opx, opy;//nilai setelah ditambah KP
int x1 = 4;//pembagi===rata" dari 4 kali sampling data
int d1, d2, d3, d4;//nilai setelah dirata"

int centerValues = 1500;
int minValues = 1000;
int maxValues = 2000;
Servo myaeleron;
Servo myelevator;
Servo mymode;
Servo myservocam;
Servo myservoarm;
Servo myservofree;
unsigned int r_a ;
unsigned int r_af ;
unsigned int r_e ;
unsigned int r_ef ;
unsigned int r_m ;
unsigned int r_t  ;
unsigned int r_i;
unsigned int r_servo1;
unsigned int r_servo2;
unsigned int r_servo3;
int aeleronvalue = 0;
int elevatorvalue = 0;
int setpoint = 120;
int kp, ki, kd;
int valuer_a = 0;
int valuer_e = 0;

void setup() {
  Serial.begin(9600);
  //sensor pin function declare
  for (int i = 0 ; i < 4 ; i++) {
    pinMode(ir[i], INPUT);
  }
  //receiver pin function declare
  pinMode(aeleronIN, INPUT);
  pinMode(elevatorIN, INPUT);
  pinMode(modeIN, INPUT);
  pinMode(triggerIN, INPUT);
  pinMode(inmode, INPUT);

  //outreceiver pin functiondeclare
  pinMode(aeleronOUT, OUTPUT);
  pinMode(elevatorOUT, OUTPUT);
  pinMode(modeOUT, OUTPUT);
  digitalWrite(aeleronOUT, LOW);
  digitalWrite(elevatorOUT, LOW);
  digitalWrite(modeOUT, LOW);

  //servoin pin function declare
  pinMode(servofree, INPUT);

  //servoout pin function declare
  pinMode(outservocam, OUTPUT);
  pinMode(outservoarm, OUTPUT);
  pinMode(outservofree, OUTPUT);

  //optional pin function declare
  pinMode(buzzer, OUTPUT);
  pinMode(lampu1, OUTPUT);
  digitalWrite(buzzer, LOW);
  digitalWrite(lampu1, LOW);

  myaeleron.attach(aeleronOUT);
  myelevator.attach(elevatorOUT);
  mymode.attach(modeOUT);
  myservocam.attach(outservocam);
  myservoarm.attach(outservoarm);
  myservofree.attach(outservofree);

  int awalaeleron = map(centerValues, 1000, 2000, 0, 180);
  int awalelevator = map(centerValues, 1000, 2000, 0, 180);
  int awalmode = map(minValues, 1000, 2000, 0, 180);
  myaeleron.write(awalaeleron);
  myelevator.write(awalelevator);
  mymode.write(awalmode);
  int awalservocam = 60;
  int awalservoarm = 60;
  int awalservofree = 60;
  myservofree.write(awalservofree);


}

void loop() {
  bacareceiver();
  bacasensor();
  mode();
  calculate();
  controllampu();
  kirimdata();
}

void bacasensor() {
  setpoint = map(setpoint, 20, 150, 0, 1023);
  temp = 0;
  temp1 = 0;
  temp2 = 0;
  temp3 = 0;
  for (int i = 0; i <= x1; i++)
  {
    left = analogRead(ir[0]);
    delay(2);
    temp = temp + left;
  }
  d1 = temp / x1;
  for (int i = 0; i <= x1; i++)
  {
    right = analogRead(ir[1]);
    delay(2);
    temp1 = temp1 + right;
  }
  d3 = temp1 / x1;
  for (int i = 0; i <= x1; i++)
  {
    front = analogRead(ir[2]);
    delay(2);
    temp2 = temp2 + front;
  }
  d2 = temp2 / x1;
  for (int i = 0; i <= x1; i++)
  {
    back = analogRead(ir[3]);
    delay(2);
    temp3 = temp3 + back;
  }
  d4 = temp3 / x1;
  /*
    Serial.println(d1);
    Serial.print(",");
    Serial.print(d2);
    Serial.print(",");
    Serial.print(d3);
    Serial.print(",");
    Serial.println(d4);
  */

}

void bacareceiver() {


  r_a = pulseIn(aeleronIN, HIGH, 24000);
  r_e = pulseIn(elevatorIN, HIGH, 24000);
  r_m = pulseIn(modeIN, HIGH, 24000);
  r_t = pulseIn(triggerIN, HIGH, 24000);
  r_i = pulseIn(inmode, HIGH, 24000);

  int i;
  int numReadings = 10;

  // Take an average of all the readings.
  for (i = 0; i < numReadings; i++) {

    valuer_a = valuer_a + r_a;

    delay(1);
  }

  valuer_a = valuer_a / numReadings;

  for (i = 0; i < numReadings; i++) {

    valuer_e = valuer_e + r_e;
    delay(1);
  }

  //Take an average of all the readings.
  valuer_e = valuer_e / numReadings;

  /*
    Serial.print(r_a);
    Serial.print(",");
    Serial.println(r_e);
    Serial.print(",");
    Serial.println(r_m);
    Serial.print(",");
    Serial.print(r_t);
    Serial.print(",");
    Serial.println(r_i);
  */

}

void calculate() {

  e1 = d1 - setpoint;
  e2 = d2 - setpoint;
  e3 = d3 - setpoint;
  e4 = d4 - setpoint;


  op1 = e1 * kp;
  op2 = e2 * kp;
  op3 = e3 * kp;
  op4 = e4 * kp;

  int f1 = op1 * 0.5;
  int f2 = op2 * 0.5;
  int f3 = op3 * 0.5;
  int f4 = op4 * 0.5;

  aeleronvalue = r_a - f3 + f1;
  elevatorvalue = r_e - f4 + f2;
  int aeleronToFc = map(aeleronvalue , minValues, maxValues, 0, 179);
  int elevatorToFc = map(elevatorvalue , minValues, maxValues, 0, 179);

  //cari nilai tengah aeleron&elevator to fc


  //  if (1400 < aeleronvalue < 1500) {
  //    myaeleron.write(90);
  //  }
  //  if (aeleronvalue > 1500) {
  //    myaeleron.write(aeleronToFc);
  //  }
  //  if (aeleronvalue < 1400) {
  //    myaeleron.write(aeleronToFc);
  //  }
  //
  //
  //  if (1400 < elevatorvalue < 1500) {
  //    myelevator.write(90);
  //  }
  //  if (elevatorvalue > 1500) {
  //    myelevator.write(elevatorToFc);
  //  }
  //  if (elevatorvalue < 1400) {
  //    myelevator.write(elevatorToFc);
  //  }



  myaeleron.write(aeleronToFc);
  myelevator.write(elevatorToFc);
  //Serial.print(aeleronvalue);
  //Serial.print(",");
  //Serial.println(elevatorvalue);

}



void mode() {

  int sensparam = 180;//nilai sensor setpoint


  //mode with collision avoidance
  if (1300 < r_i <= 1600 ) {
    digitalWrite(buzzer, LOW);
    int readmode = map(r_m, minValues, maxValues, 0, 180);
    mymode.write(readmode);
    kp = 1;
    ki = 1;
    kd = 1;

    if (d1 > sensparam) {
      digitalWrite(buzzer, HIGH);
    }
    if (d2 > sensparam) {
      digitalWrite(buzzer, HIGH);

    }

    if (d3 > sensparam) {
      digitalWrite(buzzer, HIGH);


    }
    if (d4 > sensparam)
    {
      digitalWrite(buzzer, HIGH);

    }

    if (d1 > sensparam && d2 > sensparam) {
      digitalWrite(buzzer, HIGH);

    }
    if (d1 > sensparam && d3 > sensparam) {
      digitalWrite(buzzer, HIGH);


    }
    if (d1 > sensparam && d3 > sensparam) {
      digitalWrite(buzzer, HIGH);


    }
    if (d1 > sensparam && d4 > sensparam) {
      digitalWrite(buzzer, HIGH);


    }
    if (d2 > sensparam && d3 > sensparam) {
      digitalWrite(buzzer, HIGH);

    }
    if (d2 > sensparam && d4 > sensparam) {
      digitalWrite(buzzer, HIGH);

    }
    if (d3 > sensparam && d4 > sensparam) {
      digitalWrite(buzzer, HIGH);

    }
    if (d1 > sensparam && d2 > sensparam && d3 > sensparam ) {
      digitalWrite(buzzer, HIGH);

    }

    if (d2 > sensparam && d3 > sensparam && d4 > sensparam ) {

      digitalWrite(buzzer, HIGH);


    }

    if (d1 > sensparam && d3 > sensparam && d4 > sensparam ) {
      digitalWrite(buzzer, HIGH);


    }
    if (d1 > sensparam && d2 > sensparam && d4 > sensparam ) {
      digitalWrite(buzzer, HIGH);

    }

  }

  if ( r_i >  1600) {
    digitalWrite(buzzer, LOW);
    int readmode = map(r_m, minValues, maxValues, 0, 180);
    mymode.write(readmode);
    kp = 0;
    ki = 0;
    kd = 0;

    if (d1 > sensparam) {
      digitalWrite(buzzer, HIGH);
      landingmode();

    }
    if (d2 > sensparam) {
      digitalWrite(buzzer, HIGH);
      landingmode();

    }

    if (d3 > sensparam) {
      digitalWrite(buzzer, HIGH);
      landingmode();


    }
    if (d4 > sensparam)
    {
      digitalWrite(buzzer, HIGH);
      landingmode();

    }

    if (d1 > sensparam && d2 > sensparam) {
      digitalWrite(buzzer, HIGH);

      landingmode();
    }
    if (d1 > sensparam && d3 > sensparam) {
      digitalWrite(buzzer, HIGH);
      landingmode();

    }
    if (d1 > sensparam && d3 > sensparam) {
      digitalWrite(buzzer, HIGH);

      landingmode();

    }
    if (d1 > sensparam && d4 > sensparam) {
      digitalWrite(buzzer, HIGH);

      landingmode();

    }
    if (d2 > sensparam && d3 > sensparam) {
      digitalWrite(buzzer, HIGH);

      landingmode();
    }
    if (d2 > sensparam && d4 > sensparam) {
      digitalWrite(buzzer, HIGH);
      landingmode();

    }
    if (d3 > sensparam && d4 > sensparam) {
      digitalWrite(buzzer, HIGH);

      landingmode();
    }
    if (d1 > sensparam && d2 > sensparam && d3 > sensparam ) {
      digitalWrite(buzzer, HIGH);

      landingmode();
    }

    if (d2 > sensparam && d3 > sensparam && d4 > sensparam ) {
      digitalWrite(buzzer, HIGH);


      landingmode();


    }

    if (d1 > sensparam && d3 > sensparam && d4 > sensparam ) {
      digitalWrite(buzzer, HIGH);


      landingmode();

    }
    if (d1 > sensparam && d2 > sensparam && d4 > sensparam ) {
      digitalWrite(buzzer, HIGH);


      landingmode();

    }

  }
  if (r_i <= 1300) {
    digitalWrite(buzzer, LOW);
    int readmode = map(r_m, minValues, maxValues, 0, 180);
    mymode.write(readmode);
    kp = 0;
    ki = 0;
    kd = 0;


  }




}


void landingmode() {

  mymode.write(125);

}







void kirimdata() {
  Serial.print(r_a);
  Serial.print(",");
  Serial.print(r_e);
  Serial.print(",");
  Serial.print(r_m);
  Serial.print(",");
  Serial.print(r_t);
  Serial.print(",");
  Serial.print(r_i);
  Serial.print(",");
  Serial.print(d1);
  Serial.print(",");
  Serial.print(d2);
  Serial.print(",");
  Serial.print(d3);
  Serial.print(",");
  Serial.print(d4);
  Serial.print(",");
  Serial.print(aeleronvalue);
  Serial.print(",");
  Serial.println(elevatorvalue);


}



void controllampu() {
  if (r_t <= 1400) {
    digitalWrite(lampu1, HIGH);
  }
  else {
    digitalWrite(lampu1, LOW);
  }


}


