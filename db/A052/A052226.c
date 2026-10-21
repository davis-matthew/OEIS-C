/*
 * A052226  Partial sums of A050404.
 * https://oeis.org/A052226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (8*n + 7) * C(n+6, 6) / 7 (OEIS A052226)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 55, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C(n+6, 6) */
    seq_t c = binom(n + 6, 6);

    /* Compute the multiplier (8*n + 7) */
    seq_t mult = (seq_t)8 * (seq_t)n + (seq_t)7;

    /* The product is guaranteed to be divisible by 7 */
    seq_t prod = mult * c;

    return prod / (seq_t)7;
}
