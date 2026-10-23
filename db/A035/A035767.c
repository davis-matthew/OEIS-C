/*
 * A035767  Coordination sequence for C_30 lattice.
 * https://oeis.org/A035767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^30
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 11, u128 n <= 60,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the central coefficient of the expansion of ((1+x)/(1-x))^30 */
/* by summing the product of two binomial coefficients. */
#include <stdint.h>
#include "../utils/binom.h"   /* binom(idx_t a, idx_t b) is available */

seq_t a(idx_t n)
{
    /* Compute k = 2n, the exponent of x whose coefficient we need. */
    idx_t k = 2 * n;
    /* The first factor (1+x)^30 contributes at most 30 terms. */
    idx_t max_i = (k < 30) ? k : 30;

    seq_t sum = 0;
    for (idx_t i = 0; i <= max_i; ++i)
    {
        /* C(30, i) from (1+x)^30 */
        seq_t c1 = binom(30, i);

        /* Remaining exponent for (1-x)^(-30) term. */
        idx_t m = k - i;               /* 0 <= m <= k */
        /* C(29 + m, m) from the negative binomial expansion. */
        seq_t c2 = binom(29 + m, m);

        sum += c1 * c2;
    }
    return sum;
}
