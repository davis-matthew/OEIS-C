/*
 * A090957  a(n) = 1/(Integral_{x=0..1} (x^4 - x^5)^n dx).
 * https://oeis.org/A090957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*n+1)!/(n!*(4*n)!) = binom(5*n+1, n) * (4*n+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute binomial coefficient (5*n+1 choose n) */
    seq_t c = binom((idx_t)(5*n + 1), n);

    /* Multiply by (4*n+1) to obtain the final result */
    return c * ((seq_t)(4*n + 1));
}
