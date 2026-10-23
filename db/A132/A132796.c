/*
 * A132796  Second diagonal of Gely numbers.
 * https://oeis.org/A132796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n+1)-1)/3 if n is odd * floor(2^(n+1)/3)-n if n is
 *            even
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^(n+1). */
    seq_t p = pow2(n + 1);

    if (n & 1u)                /* odd n */
    {
        /* (2^(n+1)-1)/3 */
        seq_t t = p - 1;
        t /= 3;
        return t;
    }
    else                        /* even n (including n==0) */
    {
        /* floor(2^(n+1)/3) - n */
        seq_t t = p / 3;
        t -= (seq_t)n;
        return t;
    }
}
