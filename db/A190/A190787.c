/*
 * A190787  Odd powers of 2 and 9 times odd powers of 2, sorted.
 * https://oeis.org/A190787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-3) * (17 - (-1)^n) for n > 1; a(1) = 2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 2;                     /* base case */
    if ((n & 1) == 0)                  /* even n */
        return pow2(n + 1);           /* 2^(n+1)   */
    else                               /* odd n, n > 1 */
        return (seq_t)18 * pow2(n - 3);/* 18*2^(n-3) */
}
