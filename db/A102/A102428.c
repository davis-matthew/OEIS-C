/*
 * A102428  Central column of triangle A102427.
 * https://oeis.org/A102428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(floor((5*n+2)/2), floor((n+1)/2))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute m = floor((5*n+2)/2) and k = floor((n+1)/2) */
    idx_t m = (5 * n + 2) / 2;
    idx_t k = (n + 1) / 2;
    return binom(m, k);
}
