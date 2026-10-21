/*
 * A202096  Number of (n+2) X 6 binary arrays avoiding patterns 001 and 011 in
 *          rows and columns.
 * https://oeis.org/A202096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (C(floor((n+3)/2)+3,3) * C(floor((n+2)/2)+3,3))^2
 * Range:     u8 none, u16 n <= 3, u32 n <= 17, u64 n <= 140, u128 n <= 210,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* compute alpha = floor((n+3)/2), beta = floor((n+2)/2) */
    idx_t alpha = (n + 3) / 2;
    idx_t beta  = (n + 2) / 2;

    /* binomial coefficients C(alpha+3,3) and C(beta+3,3) */
    seq_t ca = binom(alpha + 3, 3);
    seq_t cb = binom(beta + 3, 3);

    /* product and square */
    seq_t prod = ca * cb;
    return prod * prod;
}
