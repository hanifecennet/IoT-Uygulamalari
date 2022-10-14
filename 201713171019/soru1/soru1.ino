// ESP modlununu istasyon, station olarak ayarla, nete baglanip tarih bilgisini cekme
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char *ssid = "************";    // baglanilacak olan wifi aginin adi  
const char *password = "*****************";    // baglanilacak olan wifi aginin sifresi

const char *sunucu = "bilgisayar.dpu.edu.tr";         // bilgisayar.dpu.edu.tr sitesine baglanilacak
String yol="";
String sifre="";
//************************************************************
void setup() {
  Serial.begin(9600);         //seri portu baslat
  delay(1500);                // 1500 ms = 1.5sn
  Serial.println("Wifi agına baglanıyor");

  WiFi.mode(WIFI_STA);                  // istasyon modunda çalış
  WiFi.begin(ssid, password);           // SSID si yukarıda belirtilmiş olan wifi a bağlan
  
  while (WiFi.status() != WL_CONNECTED) // baglanti kuruldu mu?
  {                                     // kurulmadiysa kurulana kaddar 
    delay(100);                         // 500 ms bekle
    Serial.print(".");                  // ekrana . koy    
  }
  
  Serial.println("");
  Serial.println("Wifi erisim noktasina baglanildi");

}
//************************************************************
void loop() 
{
  String path="/tr/index/sayfa/8997/bilisim-laboratuvari";       // bilgisayar.dpu.edu.tr sitesinde bilisim-laboratuvari sayfasina gitmek icin
  WiFiClient istemci;                   // istemci nesnesini olustur
  if (!istemci.connect(sunucu, 80))     // istemci nesnesi sunucuya 80 portundan baglandi mi?
  {
    Serial.println("Sunucuya baglanamadi");
    delay(500);
    return;                              // loop un basina git
  }     

  // sunucuya baglandi, http protokolunu olustur, sayfayi istemek icin                     
  yol="GET ";
  yol+=path;
  yol+= " HTTP/1.1\r\n";
  yol+="Host: ";
  yol+=sunucu;
  yol+="\r\n";
  yol+="Connection: close\r\n\r\n";

  istemci.print(yol);                   // sayfa istegini gonder
  
  
  unsigned long simdiki_zaman = 0;
  simdiki_zaman = millis();             // millis fonksiyonu o an ki zamanı alıp simdiki_zaman değişkenine atadı. 
  
  while(!istemci.available())           // istemciye bilgi gelmemis mi?
  {                                     // gelmemis bekleme
      if(millis()-simdiki_zaman>3000)   
      { 
        istemci.stop();                 // yeniden o anki zamanı al. 7001-4000=3001>3000. 3 sn doldu
        return;                         // istemci durduruldu loop basina don
      }
  }
  //********************************************************
  while (istemci.available())           // istemciye bilgi gelmis mi?
  {
    String line = istemci.readStringUntil('\r');  // gelen veriyi satır satır oku

    
    //String filtreleme işlemi
    int var_mi=line.indexOf("201713171019:");   // okunan satirda 201713171019: indexini al
    if(var_mi!= -1)                     // index -1 ise 201713171019: yok demektir.
    {                                   // 201713171019: var ise, substring islemini yap
      
      // 14 bilgisi-> "201713171019: " sifrenin ilk karakterini almak icin
      // 16 bilgisi-> "201713171019: " sifrenin ikinci karakterini almak icin
      
      sifre=line.substring(14,16);
    }
  }
  Serial.print("Sifre: ");
  Serial.println(sifre);
  sifre="";                            
}
