/*
 * A107875  Column 1 of triangle A107873; a(n) = C( n*(n+1)/2 + n+3, n).
 * https://oeis.org/A107875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C( n*(n+3)/2 + 3, n )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute the upper index of the binomial: N = n*(n+3)/2 + 3 */
    seq_t N = (seq_t)n * (n + 3) / 2 + 3;
    return binom((idx_t)N, n);
}
