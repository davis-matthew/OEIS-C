/*
 * A031164  Irreducible Euler sums of weight 8 and depth 10+2n.
 * https://oeis.org/A031164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [C(n+8,7) - (n%2)*C((n+7)/2,3)]/8, where C is binomial
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute C(n+8,7) */
    seq_t val = binomial(n + 8, 7);

    /* If n is odd subtract C((n+7)/2,3) */
    if (n & 1U) {
        idx_t k = (n + 7) / 2;
        val -= binomial(k, 3);
    }

    /* Divide by 8 (exact division) */
    return val / 8;
}
