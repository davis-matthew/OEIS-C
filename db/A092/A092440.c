/*
 * A092440  a(n) = 2^(2*n+1) - 2^(n+1) + 1.
 * https://oeis.org/A092440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(2*n+1) - 2^(n+1) + 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t pow2n = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        pow2n *= (seq_t)2;
    }

    /* Compute 4^n = (2^n)^2 */
    seq_t pow4n = pow2n * pow2n;

    /* 2^(2*n+1) = 2 * 4^n  */
    seq_t term1 = pow4n * (seq_t)2;

    /* 2^(n+1) = 2 * 2^n */
    seq_t term2 = pow2n * (seq_t)2;

    /* a(n) = term1 - term2 + 1 */
    return term1 - term2 + (seq_t)1;
}
