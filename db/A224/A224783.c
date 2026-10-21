/*
 * A224783  Denominator of Bernoulli(n,1/2) - Bernoulli(n,0).
 * https://oeis.org/A224783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, less, easy
 * Formula:   a(n) = 2^n if n is even; a(n) = 1 if n>1 odd; a(1) = 2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)          /* special case */
        return (seq_t)2;
    if ((n & 1) == 0)    /* even n */
        return pow2(n);
    return (seq_t)1;     /* odd n > 1 */
}
