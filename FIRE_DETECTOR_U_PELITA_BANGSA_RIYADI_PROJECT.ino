#include <Sim800L.h>
#include <LiquidCrystal_I2C.h>  // libray lcd
#include <LiquidCrystal.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <DHT.h>
DHT dht(2, DHT11);

#define MQ2pin (0)
float sensorValue;

#include <SoftwareSerial.h>
const String PHONE_1 = "+6281223140348";
const String PHONE_2 = ""; //optional
const String PHONE_3 = ""; //optional

#define rxPin 2
#define txPin 3

SoftwareSerial sim800L(rxPin,txPin);
#define flame_sensor_pin 11
//fire_flag = 0 means no fire detected
boolean fire_flag = 0;
#define buzzer_pin 12
int powerPin = 3;  // untuk pengganti VCC/5vOLT
//Arduino Flame Sensor
const int buzzerPin = 12;
const int relayPin = 8;
const int flamePin = 11;
int Flame = HIGH;
int redled = 5;
int greenled = 6;
void setup() 
{
  Serial.begin(9600);
  Serial.println("Gas Sensor Warming Up!");
  delay(20000);
  lcd.begin(16, 2);
  // put your setup code here, to run once:

  pinMode(buzzerPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  pinMode(flamePin, INPUT);
  Serial.begin(9600);
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
 // jadikan pin power sebagai output
  pinMode(powerPin, OUTPUT);
// default bernilai LOW
  digitalWrite(powerPin, LOW);
  Serial.begin(9600);
  
//Begin serial communication: Arduino IDE (Serial Monitor)
  Serial.begin(115200);
  //-----------------------------------------------------------------------------------
  //Begin serial communication: SIM800L
  sim800L.begin(9600);
  //-----------------------------------------------------------------------------------
  pinMode(flame_sensor_pin, INPUT);
  
  pinMode(buzzer_pin, OUTPUT);
  digitalWrite(buzzer_pin,LOW);

  Serial.println("Initializing...");
  //Once the handshake test is successful, it will back to OK
  sim800L.println("AT");
  delay(1000);
  sim800L.println("AT+CMGF=1");
  delay(1000);
 
}

void loop()
 {
  while(sim800L.available()){
  Serial.println(sim800L.readString());
  }
  {
 digitalWrite(powerPin, HIGH);
 float kelembaban = dht.readHumidity();
 float suhu = dht.readTemperature();

 Serial.print("kelembaban: ");
 Serial.print(kelembaban);
 Serial.print(" ");
 Serial.print("suhu: ");
 Serial.println(suhu);
 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Kelembaban: ");
  lcd.setCursor(11,0);
  lcd.print(kelembaban);
  
  lcd.init();

  
  lcd.setCursor(0,1);
  lcd.print("Suhu: ");
  lcd.setCursor(5,1);
  lcd.print(suhu);
  delay(1000); 
  }
  int flame_value = digitalRead(flame_sensor_pin);
  // put your main code here, to run repeatedly:
  
  if (Flame == LOW)
   {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(relayPin, HIGH);
    digitalWrite(redled, HIGH);
    digitalWrite(greenled, LOW);
    
   if(fire_flag == 0)
    {
      Serial.println("Fire Detected.");
      fire_flag == 1;
      send_multi_sms();
      make_multi_call();
    }
  }
   // put your main code here, to run repeatedly:  
sensorValue=analogRead(MQ2pin);
Serial.print("Sensor Value:");
Serial.print(sensorValue);

Flame = digitalRead(flamePin);
  if (Flame== LOW || sensorValue > 1025)
    
  {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(relayPin, HIGH);
    digitalWrite(redled, HIGH);
    digitalWrite(greenled, LOW);
    
  }
else
  {
    digitalWrite(buzzer_pin,LOW);
    fire_flag = 0;
    
    digitalWrite(buzzerPin, LOW);
    digitalWrite(relayPin, LOW);
    digitalWrite(greenled, HIGH);
    digitalWrite(redled, LOW);
    lcd.print("~AMAN");
  }
  lcd.clear();

  lcd.setCursor(0, 1);
  lcd.print("PROJECT ARDUINO");
  delay(1000);
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("U-PELITA BANGSA");
  delay(1000);
  lcd.backlight();

  digitalWrite(powerPin, HIGH);
}
 void send_multi_sms()
{
  if(PHONE_1 != ""){
    Serial.print("Phone 1: ");
    send_sms("Fire is Detected", PHONE_1);
  }
  if(PHONE_2 != ""){
    Serial.print("Phone 2: ");
    send_sms("Fire is Detected", PHONE_2);
  }
  if(PHONE_3 != ""){
    Serial.print("Phone 3: ");
    send_sms("Fire is Detected", PHONE_3);
  }
}

void make_multi_call()
{
  if(PHONE_1 != ""){
    Serial.print("Phone 1: ");
    make_call(PHONE_1);
  }
  if(PHONE_2 != ""){
    Serial.print("Phone 2: ");
    make_call(PHONE_2);
  }
  if(PHONE_3 != ""){
    Serial.print("Phone 3: ");
    make_call(PHONE_3);
  }
}

void send_sms(String text, String phone)
{
    Serial.println("sending sms....");
    delay(50);
    sim800L.print("AT+CMGF=1\r");
    delay(1000);
    sim800L.print("AT+CMGS=\""+phone+"\"\r");
    delay(1000);
    sim800L.print(text);
    delay(100);
    sim800L.write(0x1A); 
    delay(5000);
}

void make_call(String phone)
{
    Serial.println("calling....");
    sim800L.println("ATD"+phone+";");
    delay(20000); //20 sec delay
    sim800L.println("ATH");
    delay(1000); //1 sec delay
}


