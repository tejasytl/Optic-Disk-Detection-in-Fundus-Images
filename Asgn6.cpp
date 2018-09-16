#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int edgeThresh = 1;
int lowThreshold = 0;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
Mat test_1;

void CannyThreshold(int, void*)
{
  Mat detected_edges;
  Mat dst;
  blur(test_1, detected_edges, Size(3,3));
  Canny(detected_edges, detected_edges, 16, 48, 3);
  dst = Scalar::all(0);
  test_1.copyTo(dst, detected_edges);
  ////imshow("Output Image 1", dst);
  cout<<lowThreshold<<endl;
 }

void ip_termp (int file)
 {
   string name = "./DRIVE/training/images/";
   string index = to_string(file);

   Mat image = imread(name+index+"_training.tif", CV_LOAD_IMAGE_COLOR);
   //VideoCapture cap("/Users/Piyush_Jena/Downloads/DRIVE/training/mask/25_training_mask.gif");
   Mat mask;

   /*cap.read(mask);
   cvtColor(mask, mask, cv::COLOR_BGR2GRAY);*/

//   Mat test = image.clone();
   Mat test = image.clone();
   Mat YUV[3];

   cvtColor(image, test, CV_BGR2YUV);
   split(test,YUV);

   test_1 = YUV[0].clone();
   //imshow("YComponent" + to_string(file), test_1);

   Mat detected_edges;
   blur(test_1, detected_edges, Size(3,3));
   Canny(detected_edges, detected_edges, 16, 48, 3);
   //imshow("detected_edges" + to_string(file), detected_edges);
   threshold(YUV[0], mask, (int)(255*0.1), 255, THRESH_BINARY);
   test_1 = mask - test_1;
   //imshow("thresholded" + to_string(file), test_1);

   //threshold(YUV[0], test_1, (int)(255*0.9), 255, THRESH_BINARY);

   Mat element = getStructuringElement(MORPH_ELLIPSE ,Size(11, 11), Point(5, 5));
   erode(test_1, test_1, element);
   medianBlur (test_1, test_1, 5 );
   //imshow("Erosion" + to_string(file), test_1);
   //test_1 = mask - test_1;
   //equalizeHist(test_1, test_1);
   ////imshow("equalizeHist" + to_string(file), test_1);

   vector<Vec3f> circles;
   HoughCircles(test_1, circles, HOUGH_GRADIENT, 1, 100, 48, 16, 25, 60);
   cout<<circles.size()<<endl;
   for(size_t i = 0; i < circles.size(); i++ )
   {
     Vec3i c = circles[i];
     Point center = Point(c[0], c[1]);
     //circle(image, center, 1, Scalar(0,100,100), 2, LINE_AA);
     int radius = c[2];
     int mean = 0;
     for(int j = -0.5*radius ; j < 0.5*radius ; j++)
       for(int k = -0.5*radius ; k < 0.5*radius ; k++)
         mean += (int)test_1.at<uchar>(Point(c[0]+j,c[1]+k));
     mean /= (c[2]*c[2]);
     cout<<mean<<endl;

     if(mean < 255*0.5)
       circle(image, center, radius, Scalar(255,0,0), 2, LINE_AA);

   }
   imshow("detected circles" + to_string(file), image);
   waitKey(10);
 }

 void ip_termp_test (int file)
 {
   string name = "./DRIVE/test/images/";
   string index;
   if(file<10)
    index = "0"+to_string(file);
   else
    index = to_string(file);

   Mat image = imread(name+index+"_test.tif", CV_LOAD_IMAGE_COLOR);
   //VideoCapture cap("/Users/Piyush_Jena/Downloads/DRIVE/training/mask/25_training_mask.gif");
   Mat mask;

   /*cap.read(mask);
   cvtColor(mask, mask, cv::COLOR_BGR2GRAY);*/

   Mat test = image.clone();
   Mat YUV[3];

   cvtColor(image, test, CV_BGR2YUV);
   split(test,YUV);

   test_1 = YUV[0].clone();
   //imshow("YComponent" + to_string(file), test_1);

   Mat detected_edges;
   blur(test_1, detected_edges, Size(3,3));
   Canny(detected_edges, detected_edges, 16, 48, 3);
   //imshow("detected_edges" + to_string(file), detected_edges);
   threshold(YUV[0], mask, (int)(255*0.1), 255, THRESH_BINARY);
   test_1 = mask - test_1;
   //imshow("thresholded" + to_string(file), test_1);

   //threshold(YUV[0], test_1, (int)(255*0.9), 255, THRESH_BINARY);

   Mat element = getStructuringElement(MORPH_ELLIPSE ,Size(11, 11), Point(5, 5));
   erode(test_1, test_1, element);
   medianBlur (test_1, test_1, 5 );
   //imshow("Erosion" + to_string(file), test_1);
   //test_1 = mask - test_1;
   //equalizeHist(test_1, test_1);
   ////imshow("equalizeHist" + to_string(file), test_1);

   vector<Vec3f> circles;
   HoughCircles(test_1, circles, HOUGH_GRADIENT, 1, 100, 48, 16, 25, 60);
   cout<<circles.size()<<endl;
   for(size_t i = 0; i < circles.size(); i++ )
   {
     Vec3i c = circles[i];
     Point center = Point(c[0], c[1]);
     //circle(image, center, 1, Scalar(0,100,100), 2, LINE_AA);
     int radius = c[2];
     int mean = 0;
     for(int j = -0.5*radius ; j < 0.5*radius ; j++)
       for(int k = -0.5*radius ; k < 0.5*radius ; k++)
         mean += (int)test_1.at<uchar>(Point(c[0]+j,c[1]+k));
     mean /= (c[2]*c[2]);
     cout<<mean<<endl;

     if(mean < 255*0.5)
       circle(image, center, radius, Scalar(255,0,0), 2, LINE_AA);

   }
   imshow("detected circles" + to_string(file), image);
   waitKey(10);
 }


int main()
{

	int val = 0;

	while(val != -1)
	{
		cout << "Enter Image Number between 1 to 40 (both inclusive). Enter -1 to quit: ";
		cin >> val;

		if(val >= 1 && val <= 20)
		{
			ip_termp_test(val);
		}
		else if(val >= 21 && val <= 40)
		{
			ip_termp(val);
		}
		else if(val != -1)
		{
			cout  << "Invalid Image Number" << endl;
			cout << "Enter Image Number between 1 to 40 (both inclusive). Enter -1 to quit: ";
		}
		//waitKey(0);
	}

	/*
  for(int i = 21 ; i <= 40 ; i++)
  {
    ip_termp(i);
  }

  for(int i = 1; i <= 20 ; i++)
  {
    ip_termp_test(i);
  }

  waitKey(0);
  */
  return 0;
}
