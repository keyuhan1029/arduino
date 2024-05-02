const byte stoplight=3;  //IN2

// 左馬達控制設定
const byte LEFT1 = 8;  //IN1
const byte LEFT2 = 9;  //IN2
const byte LEFT_PWM = 10;

// 右馬達控制設定
const byte RIGHT1 = 7;  //IN3
const byte RIGHT2 = 6;  //IN4
const byte RIGHT_PWM = 5;

int trigPin = 11;    // 超音波感測器 Trig腳接 Arduino pin 11
int echoPin = 12;
long duration, cm ;

// 設定PWM輸出值（代表的是車子的速度）
byte motorSpeed = 100;

void forward() {  // 前進
  //左輪
  digitalWrite(LEFT1, HIGH);
  digitalWrite(LEFT2, LOW);
  analogWrite(LEFT_PWM, motorSpeed);
  
  //右輪。因在小車上馬達安裝方向左右兩個是相反的，所以另一隻馬達的設定要相反，兩輪才能配合。）
  digitalWrite(RIGHT1, LOW);
  digitalWrite(RIGHT2, HIGH);

  analogWrite(RIGHT_PWM, motorSpeed+15);
  digitalWrite(stoplight, LOW);

}

void backward() { // 後退
  digitalWrite(LEFT1, LOW);
  digitalWrite(LEFT2, HIGH);
  analogWrite(LEFT_PWM, motorSpeed);

  digitalWrite(RIGHT1, HIGH);
  digitalWrite(RIGHT2, LOW);
  analogWrite(RIGHT_PWM, motorSpeed);
}

void turnLeft(int cm) { // 左轉
  //左輪不動，右輪動（速度為0）
  
  analogWrite(LEFT_PWM, 0);
  digitalWrite(LEFT2, HIGH);

  digitalWrite(RIGHT1, LOW);
  digitalWrite(RIGHT2, HIGH);
  analogWrite(RIGHT_PWM, motorSpeed-10);
  if (cm <= 5) {
    stopMotor();
    delay (20);
   }
}

void turnRight(int cm) {  // 右轉
  //右輪不動，左輪動（速度為0）
  analogWrite(LEFT_PWM, motorSpeed-40);
  digitalWrite(RIGHT1, HIGH);

  digitalWrite(LEFT1, HIGH);
  digitalWrite(LEFT2, LOW);
 

  analogWrite(RIGHT_PWM, 0);
  if (cm <= 5) {
    stopMotor();
    delay (20);
   }
}

void stopMotor() {  //停止，兩輪速度為0
  digitalWrite(stoplight, HIGH);
  analogWrite(LEFT_PWM, 0);
  analogWrite(RIGHT_PWM, 0);
}

void setup() {
  //設定每一個PIN的模式
  pinMode(LEFT1, OUTPUT);
  pinMode(LEFT2, OUTPUT);
  pinMode(LEFT_PWM, OUTPUT);
  pinMode(RIGHT1, OUTPUT);
  pinMode(RIGHT2, OUTPUT);
  pinMode(RIGHT_PWM, OUTPUT);
  Serial.begin (9600);           //設定序列埠監控視窗 (Serial Monitor) 和 Arduino資料傳輸速率為 9600 bps (Bits Per Second)
  pinMode(trigPin, OUTPUT);      //Arduino 對外啟動距離感測器Trig腳，射出超音波 
  pinMode(echoPin, INPUT); 
  pinMode(stoplight, OUTPUT);  //超音波被障礙物反射後，Arduino讀取感測器Echo腳的時間差
}

long time = 0;
long stop = 0;

void loop() {
  //本範例會讓車子向前、向後、向左、向右、停止，各2秒，然後不斷重複
  forward();
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1; 
  

  int sensorValue = analogRead(A0);
  
  if(sensorValue > 150){
    turnLeft(cm);
    delay(200);
    time += 200;
  }
  if(sensorValue < 150 && time != 0){
    forward();
    delay(1500);
    Serial.println(time); 
    turnRight(cm);
    delay(time);
    time = 0;
  }
  if(cm <= 10){
    stopMotor();
  
    delay(20);
  }
  delay(10);
}