/*
 * A035774  Coordination sequence for C_37 lattice.
 * https://oeis.org/A035774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^37
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 10, u128 n <= 40,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Precompute the binomial coefficients *binom(37,i)* in a static table */
    static seq_t binom37[38];
    static bool init = false;

    if (!init) {
        for (idx_t i = 0; i <= 37; ++i)
            binom37[i] = binomial(37, i);
        init = true;
    }

    /* m = 2n, the exponent of x we want from the generating function */
    idx_t m = n << 1;
    idx_t max_i = (m < 37) ? m : 37;

    seq_t sum = 0;

    for (idx_t i = 0; i <= max_i; ++i) {
        /* binomial(m - i + 36, 36) gives the coefficient from (1 - x)^-37   */
        idx_t a = m - i + 36;            /* m - i + 36 is always ≤ m + 36 ≤ 2n + 36 */
        seq_t term = binom37[i] * binomial(a, 36);
        sum += term;
    }

    return sum;
}
