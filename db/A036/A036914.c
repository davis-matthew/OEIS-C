/*
 * A036914  a(n) = binomial(2*n,n)*binomial(3*n,2*n)^4.
 * https://oeis.org/A036914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n,n) * binomial(3*n,2*n)^4
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* binomial(2*n, n) */
    seq_t c1 = binom(2 * n, n);

    /* binomial(3*n, 2*n) */
    seq_t c2 = binom(3 * n, 2 * n);

    /* (c2^2)^2 = c2^4 */
    seq_t c2_sq   = c2 * c2;
    seq_t c2_pow4 = c2_sq * c2_sq;

    /* final product */
    return c1 * c2_pow4;
}
