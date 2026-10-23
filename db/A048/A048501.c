/*
 * A048501  a(n) = 2^(n-1)*(8*n-14) + 8.
 * https://oeis.org/A048501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(8*n-14) + 8 (with a(0)=1, a(1)=2)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Special cases required because 8*n-14 is negative for n=0,1 */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    /* For n >= 2, 8*n-14 is positive */
    seq_t p = pow2(n - 1);          /* 2^(n-1) */
    seq_t mult = (seq_t)8 * n - (seq_t)14;  /* 8*n - 14, safe for n>=2 */

    return p * mult + (seq_t)8;
}
