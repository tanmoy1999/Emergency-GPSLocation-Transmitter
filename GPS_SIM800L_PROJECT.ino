#include <SoftwareSerial.h>
#include <TinyGPS++.h>
SoftwareSerial sim800l(9, 8);
SoftwareSerial GPS_SoftSerial(4, 3);

TinyGPSPlus gps;

volatile float minutes, seconds;
volatile int degree, secs, mins;
String Data_SMS;            //String that we're going to send via SMS
float t, h;

void setup() {
  // put your setup code here, to run once:
  sim800l.begin(9600);   //Module baude rate, this is on max, it depends on the version
  GPS_SoftSerial.begin(9600);
  Serial.begin(9600);


  smartDelay(1000);
  unsigned long start;
  double lat_val, lng_val, alt_m_val;
  bool loc_valid, alt_valid;
  lat_val = gps.location.lat();
  loc_valid = gps.location.isValid();
  lng_val = gps.location.lng();
  alt_m_val = gps.altitude.meters();
  alt_valid = gps.altitude.isValid();

  if (!loc_valid)
  {
    setup();
  }
  else
  {
    Serial.println("GPS READING: ");
    DegMinSec(lat_val);
    Serial.print("Latitude in Decimal Degrees : ");
    Serial.println(lat_val, 6);

    DegMinSec(lng_val);
    Serial.print("Longitude in Decimal Degrees : ");
    Serial.println(lng_val, 6);
    delay(4000);
  }

  h = lng_val;
  t = lat_val;
  Send_DHT_Data();
  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void Send_DHT_Data()
{
  Serial.println("Sending Data...");     //Displays on the serial monitor
  sim800l.print("AT+CMGF=1\r");          // Set the shield to SMS mode
  delay(100);
  sim800l.print("AT+CMGS=\"+916290107993\"\r");  //Your phone number don't forget to include your country code example +91xxxxxxxxx"
  delay(500);
  Data_SMS = "Lat = " + String(t, 6) + "\nLng =" + String(h, 6) + "\nhttps://maps.google.com/?q=" + String(t, 6) + "," + String(h, 6) + " HELP"; //A string to regroup the whole message as it's composed of Strings and Float --> to a single string,


  sim800l.print(Data_SMS);  //This string is sent as SMS
  delay(500);
  sim800l.print((char)26);//(required according to the datasheet)
  delay(500);
  sim800l.println();
  Serial.println("Data Sent.");
  delay(500);

}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (GPS_SoftSerial.available())
      gps.encode(GPS_SoftSerial.read());
  } while (millis() - start < ms);
}

void DegMinSec( double tot_val)
{
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
}
