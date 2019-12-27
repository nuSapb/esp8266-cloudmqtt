#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "WIFI SSID"; // wifi ssid
const char* password = "WIFI PASSWORD"; // wifi password

// Config MQTT Server
const char * topic = "/server"; // topic
#define mqtt_server "mqtt.cloudmqtt.com" // server
#define mqtt_port xxxxxxx // port number
#define mqtt_user "user" // user
#define mqtt_password "password" // password


#define LED_PIN D7 // using port D7 connect to LED
char *led_status = "OFF";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // connect the network
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  client.setServer(mqtt_server, mqtt_port); // connect to mqtt server
  client.setCallback(callback); // create callback function when mqtt connect
  }
  
  void loop() {
  if (!client.connected()) {
    Serial.print("MQTT connecting...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      client.subscribe(topic); // topic name to follow
      Serial.println("connected");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 sec");
      delay(5000); // delay time for 5 sec
      return;
    }
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message from ");
  Serial.println(topic);
  String msg = "";
  int i = 0;
  while (i < length) {
    msg += (char)payload[i++]; // reading message from following topic
  }

  Serial.print("receive ");
  Serial.println(msg); // show message from following topic on serial
  
  if (msg == "on") {
    digitalWrite(LED_PIN, 1);
    led_status = "NodeMCU : LED ON";
    client.publish("/NodeMCU", led_status); // send message back to topic 
  } else if (msg == "off") {
    digitalWrite(LED_PIN, 0);
    led_status = "NodeMCU : LED OFF";
    client.publish("/NodeMCU", led_status);
    }else if(msg=="status"){
    client.publish("/NodeMCU", led_status);
  }
  
  if (led_status != "") {
  Serial.print("Status LED ");
  Serial.println(led_status);
  }
}
