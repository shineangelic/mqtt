/*
 *  This sketch sends a message to a TCP server
 *  go to http://www.hivemq.com/demos/websocket-client/ to test msgs
 *
 */
#define mqtt_server "broker.hivemq.com"
#define mqtt_user "your_username"
#define mqtt_password "your_password"
#define watts_topic "shine/sensor/watts"
#include <WiFi.h>
#include <PubSubClient.h>
#include "SSD1306.h"
char ssid[] = "Cisco66778"; //  your network SSID (name of wifi network)
char pass[] = "cimadaconegliano";    // your network password

WiFiClient wclient;
PubSubClient client(wclient);

SSD1306  display(0x3c, 4, 15);

void setup()
{
    pinMode(16,OUTPUT);
    digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
    delay(50); 
    digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high
    Serial.begin(115200);
    delay(10);

    // Initialising the UI will init the display too.
    display.init();
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_10);

    WiFi.begin(ssid, pass);
    Serial.println();
    Serial.print("Wait for WiFi... ");

     // attempt to connect to Wifi network:
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      // wait 1 second for re-trying
      delay(1000);
    }
    display.drawString(0, 10, "Setup complete, IP address:");
    display.drawString(0, 20, ipToString(WiFi.localIP()));
    display.display();
    delay(500);
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    client.setServer(mqtt_server, 1883);
   


    display.drawString(0, 30,  "MQTT Server:");
    display.drawString(0, 40,  mqtt_server);
    display.display();
    delay(2500);
}

String ipToString(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    display.clear();
    Serial.print("Attempting MQTT connection...");
    display.drawString(0, 10, "MQTT connection...");
    display.display();
    delay(500);
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    //if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      display.drawString(0, 20, "failed, rc=");
      display.drawString(44, 20, String(client.state()));
      // Wait 5 seconds before retrying
      display.display();
      delay(5000);
    }
    display.display();
  }
}
long lastMsg = 0;
long cnt = 0;
void loop()
{
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
    // clear the display
    display.clear();
    long now = millis();
    if (now - lastMsg > 1000) {
      lastMsg = now;
      cnt++;
      client.publish(watts_topic, String(cnt).c_str(), true);
    }
    display.drawString(0, 10, "Published msgs:");
    display.drawString(85, 10, String(cnt));
    display.display();
     
}

