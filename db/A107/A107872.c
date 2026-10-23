/*
 * A107872  Column 1 of triangle A107870; a(n) = C(n*(n+1)/2 + n+2, n).
 * https://oeis.org/A107872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n*(n+1)/2 + n + 2, n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C( n*(n+1)/2 + n + 2 , n ) */
    idx_t N = ((idx_t)n * ((idx_t)n + 1)) / 2 + n + 2;
    return binomial(N, n);
}
