/*
 * A215088  a(n)=Sum{d(i)*2^i: i=0,1,...,m}, where Sum{d(i)*5^i: i=0,1,...,m}
 *          is the base 5 representation of n.
 * https://oeis.org/A215088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum{ d(i) * 2^i : i = 0,1,...,m }, where n = Sum{ d(i) *
 *            5^i } is the base-5 expansion of n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: convert n to base 5 and evaluate the weighted
   sum using powers of 2.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t bit    = 1;  /* 2^i, starts at i = 0 */

    while (n > 0) {
        /* n % 5 fits in an idx_t, cast to int for simplicity */
        int digit = (int)(n % 5);
        result += (seq_t)digit * bit;
        n /= 5;
        bit <<= 1;        /* next power of 2 */
    }

    return result;
}
