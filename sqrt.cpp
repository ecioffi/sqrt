#include <iostream>
#include <cmath>
#include <chrono>


using namespace std;
using namespace std::chrono;

double intToFP(unsigned int input)
{
	double result = 0;
	for (int bit = 0; bit < 32; bit++)
	{
		if (input & (1U << bit))
		{
			result += std::pow(2.0, bit - 14);
		}
	}
	
	return result;
}

unsigned int mySqrt(unsigned int input)
{
	unsigned int minSq = 0;
	unsigned int maxSq = 1U << 30;
	unsigned int minRoot = 0;
	unsigned int maxRoot = 1U << 15;

	unsigned int root = 0;
	unsigned int sq = 0;

	unsigned int offset = 1U << 28;

	for (int i = 0; i < 31; i++)
	{

		//std::cout << root << std::endl;
		//std::cout << sq << "\n" << std::endl;

		root = (minRoot + maxRoot) >> 1;
		sq = ((minSq + maxSq) >> 1) - offset;
		offset >>= 2;

		if (sq > input)
		{
			maxSq = sq;
			maxRoot = root;
		}
		else
		{
			minSq = sq;
			minRoot = root;
		}
		//else
			//cout << "atari " << i << endl;
			//return root;
	}

	return root;
}

uint32_t comp(uint32_t a_nInput)
{
    uint32_t op  = a_nInput;
    uint32_t res = 0;
    uint32_t one = 1uL << 30; // The second-to-top bit is set: use 1u << 14 for uint16_t type; use 1uL<<30 for uint32_t type


    // "one" starts at the highest power of four <= than the argument.
    while (one > op)
    {
        one >>= 2;
    }

    while (one != 0)
    {
        if (op >= res + one)
        {
            op = op - (res + one);
            res = res +  (one << 1);
        }
        res >>= 1;
        one >>= 2;
    }
    return res;
}

int main()
{
	unsigned int num = 46557666U;

	double result = std::pow(2.0, 7) * intToFP(mySqrt(num));
	std::cout << "Sqrt(" << intToFP(num) << ") = " << sqrt(intToFP(num)) << "\nAlgorithim gives " << result << std::endl;
	result = std::pow(2.0, 7) * intToFP(comp(num));
	std::cout << "Sqrt(" << intToFP(num) << ") = " << sqrt(intToFP(num)) << "\nAlgorithim gives " << result << std::endl;

	milliseconds t1 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	for (int i = 0; i < 1000000; i++)
		comp(num);
	milliseconds t2 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	milliseconds t3 = t2 - t1;
	cout << t3.count() << endl;
	t1 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	for (int i = 0; i < 1000000; i++)
		mySqrt(num);
	t2 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	t3 = t2 - t1;
	cout << t3.count() << endl;
	return 0;
}
