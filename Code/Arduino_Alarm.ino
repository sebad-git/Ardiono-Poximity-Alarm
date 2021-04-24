
const byte IR_SENSOR=A0;
const byte LIGHTS_PIN=A1;
const byte ALARM=A2;
const byte ACTIVATED_PIN=A3;
const byte DISTANCE_SENSORS[] = {7,7};

const float CMS_CONVERTER = 0.01723;
const byte TRIGGER_DISTANCE = 100;
unsigned long currentMillis;

long DISTANCE = 500;
bool ACTIVATED = false;
bool IR_PRESSED = false;
//Alarm.
unsigned long previousMillis = 0;
const long interval = 200;
int ALARM_STATE = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(IR_SENSOR,INPUT);
  pinMode(LIGHTS_PIN,OUTPUT);
  pinMode(ACTIVATED_PIN,OUTPUT);
  pinMode(ALARM,OUTPUT);
}

void loop(){
  currentMillis = millis();
  readRemoteSignal();
  readDistanceSensor();
  //Serial.print(DISTANCE); Serial.println("cm");
  if(ACTIVATED){
    if(DISTANCE<=TRIGGER_DISTANCE){ soundAlarm(); }
    else{ stopAlarm(); }
  }
}
void readRemoteSignal(){
  if(digitalRead(IR_SENSOR)==LOW && !IR_PRESSED) {
	IR_PRESSED=true;
    ACTIVATED=!ACTIVATED;
    digitalWrite(ACTIVATED_PIN,ACTIVATED?HIGH:LOW);
    delay(500);
    Serial.print("ACTIVATED:"); Serial.println(ACTIVATED?"ON":"OFF");
  }else{ IR_PRESSED=false; }
}

void readDistanceSensor() {
  pinMode(DISTANCE_SENSORS[0], OUTPUT);  // Clear the trigger
  digitalWrite(DISTANCE_SENSORS[0], LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(DISTANCE_SENSORS[0], HIGH);
  delayMicroseconds(10);
  digitalWrite(DISTANCE_SENSORS[0], LOW);
  pinMode(DISTANCE_SENSORS[1], INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  //return 0.01723 * pulseIn(DISTANCE_SENSORS[1], HIGH);
  DISTANCE = CMS_CONVERTER * pulseIn(DISTANCE_SENSORS[1], HIGH);
}

void soundAlarm(){
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ALARM_STATE = ALARM_STATE==LOW? HIGH:LOW;
  	digitalWrite(LIGHTS_PIN,ALARM_STATE); 
    tone(ALARM, 1300, 100);
  }
  
}

void stopAlarm(){
  ALARM_STATE=LOW;
  digitalWrite(ALARM,LOW);
  digitalWrite(LIGHTS_PIN,LOW);
}