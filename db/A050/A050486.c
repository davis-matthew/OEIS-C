/*
 * A050486  a(n) = binomial(n+6,6)*(2n+7)/7.
 * https://oeis.org/A050486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+6,6)*(2*n+7)/7
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 59, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute binomial(n+6,6) */
    idx_t nplus6 = n + 6;
    seq_t binom = binomial(nplus6, 6);

    /* Compute (2*n + 7) in seq_t domain */
    seq_t factor = ((seq_t)2 * (seq_t)n) + (seq_t)7;

    /* Final value: binom * factor / 7 */
    return (binom * factor) / (seq_t)7;
}
