/*
 * A213245  Number of nonzero elements in GF(2^n) that are 7th powers.
 * https://oeis.org/A213245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n-1)/gcd(2^n-1,7)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Since 7 divides 2^n-1 iff n is a multiple of 3, the result can be computed
   as (2^n-1)/7 for n%3==0, otherwise simply 2^n-1. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute M = 2^n - 1 */
    seq_t m = pow2(n);   /* 2^n */
    --m;                 /* 2^n - 1 */

    /* If n is a multiple of 3, divide by 7 */
    if ((n % 3) == 0)
        return m / 7;

    /* Otherwise, return M itself */
    return m;
}
