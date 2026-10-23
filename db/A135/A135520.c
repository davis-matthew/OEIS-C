/*
 * A135520  a(n) = 4*a(n-2) starting with a(0)=2 and a(1)=1.
 * https://oeis.org/A135520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2^(n+(-1)^n) (equivalent to a(0)=2, a(1)=1,
 *            a(n)=4*a(n-2))
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n even, (-1)^n = 1 -> exponent = n+1.
       For n odd,  (-1)^n = -1 -> exponent = n-1. */
    if ((n & 1) == 0)
        return pow2(n + 1);
    else
        return pow2(n - 1);
}
