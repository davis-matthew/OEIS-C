/*
 * A035782  Coordination sequence for C_45 lattice.
 * https://oeis.org/A035782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^45
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 30,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Pre‑compute the 45‑choose‑k values once. */
    static seq_t C45[46];
    static char init = 0;
    if (!init) {
        for (int k = 0; k <= 45; ++k)
            C45[k] = binom(45, (idx_t)k);            /* small numbers, fit in seq_t */
        init = 1;
    }

    seq_t result = 0;
    idx_t maxk = (2 * n < 45) ? (2 * n) : 45;   /* k cannot exceed 2n */

    for (idx_t k = 0; k <= maxk; ++k) {
        /* exponent of (1-x)^(-45) is (2n - k). */
        idx_t m = 44 + (2 * n - k);
        seq_t term = C45[k] * binom(m, 44);
        result += term;
    }

    return result;
}
