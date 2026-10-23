/*
 * A027811  a(n) = 3*(n+1)*binomial(n+5,6).
 * https://oeis.org/A027811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n+1)*binomial(n+5,6)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 49, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 1: a(1) is first term */
    seq_t factor = ((seq_t)n + 1) * 3;
    return factor * binomial(n + 5, 6);
}
