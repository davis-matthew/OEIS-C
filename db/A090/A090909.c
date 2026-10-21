/*
 * A090909  Terms a(k) of A073869 for which a(k-1) = a(k), and a(k) and a(k+1)
 *          are distinct.
 * https://oeis.org/A090909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(phi^2 * n) where phi = (1 + sqrt(5)) / 2 is the
 *            golden ratio
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The formula can be written as a(n) = floor( (3*n + floor(n*sqrt(5))) / 2 ). */

#include <stdint.h>

/* Integer square root for unsigned __int128 using a binary search based on division.
   This works safely for all values that fit in an unsigned __int128. */
static unsigned __int128 int128_sqrt(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = (((unsigned __int128)1) << 64) - 1;
    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        if (mid <= x / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Compute floor(n * sqrt(5)) as floor(sqrt(5*n^2)) */
    unsigned __int128 nn   = (unsigned __int128)n * (unsigned __int128)n;  /* n^2 */
    unsigned __int128 prod = nn * (unsigned __int128)5;                    /* 5 * n^2 */
    unsigned __int128 sqn5 = int128_sqrt(prod);                            /* floor(sqrt(5*n^2)) */

    /* Compute (3*n + floor(n*sqrt5)) / 2 */
    unsigned __int128 numerator   = (unsigned __int128)3 * (unsigned __int128)n + sqn5;
    unsigned __int128 result128   = numerator / ((unsigned __int128)2);

    return (seq_t)result128;
}
