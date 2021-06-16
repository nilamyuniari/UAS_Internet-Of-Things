#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define lampu D1   //Relay 1
#define led D3 //Relay 2
#define kipas D4 //Relay 3
#define garage D5 //Relay 4

const char *ssid = "iPhone";         //silakan disesuaikan sendiri
const char *password = "terserahnilam"; //silakan disesuaikan sendiri

const char *mqtt_server = "broker.emqx.io";

WiFiClient espClient;
PubSubClient client(espClient);


unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi()
{

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String messageTemp;
  for (int i = 0; i < length; i++)
  {

    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  
  Serial.println();

  // Switch on the LED if an 1 was received as first character

  if (String(topic) == "room/lampu")
  {
    if (messageTemp == "true")
    {
      Serial.println("Lampu Nyala");
      digitalWrite(lampu, LOW);
    }
    else
    {
      Serial.println("Lampu Mati");
      digitalWrite(lampu, HIGH);
    }
  }
  if (String(topic) == "room/led")
  {
    if (messageTemp == "true")
    {
      Serial.println("LED Nyala");
      digitalWrite(led, LOW);
    }
    else
    {
      Serial.println("LED Mati");
      digitalWrite(led, HIGH);
    }
  }
  if (String(topic) == "room/kipas")
  {
    if (messageTemp == "true")
    {
      Serial.println("Kipas Nyala");
      digitalWrite(kipas, LOW);
    }
    else
    {
      Serial.println("Kipas Mati");
      digitalWrite(kipas, HIGH);
    }
  }
  if (String(topic) == "room/garage")
  {
    if (messageTemp == "true")
    {
      Serial.println("Garage Nyala");
      digitalWrite(garage, LOW);
    }
    else
    {
      Serial.println("Garage Mati");
      digitalWrite(garage, HIGH);
    }
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      client.subscribe("room/lampu");
      client.subscribe("room/kipas");
      client.subscribe("room/led");
      client.subscribe("room/garage");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(lampu, OUTPUT); //atur pin-pin digital sebagai output
  pinMode(led, OUTPUT);
  pinMode(kipas, OUTPUT);
  pinMode(garage, OUTPUT);
}

void loop()
{

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}