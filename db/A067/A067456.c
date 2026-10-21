/*
 * A067456  Floor( sqrt( sum of the decimal digits of n squared)).
 * https://oeis.org/A067456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor(sqrt( sum over decimal digits of n of (digit)^2 ))
 * Range:     u8 n <= 7, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    idx_t tmp = n;
    seq_t sum = 0;          /* sum of squares of decimal digits of n */
    while (tmp > 0)
    {
        unsigned digit = (unsigned)(tmp % 10);
        sum += (seq_t)(digit * digit);
        tmp /= 10;
    }
    return floor_sqrt(sum);
}
