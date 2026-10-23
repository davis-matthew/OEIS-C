/*
 * A014366  Inverse of 357th cyclotomic polynomial.
 * https://oeis.org/A014366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of xⁿ in 1/Φ_357(x) (period 357)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define PERIOD 357

seq_t a(idx_t n)
{
    static seq_t cache[PERIOD];
    static int ready = 0;

    if (!ready) {
        seq_t g[PERIOD] = {0};
        seq_t h[PERIOD];
        unsigned i;

        /* start with series = 1 */
        g[0] = (seq_t)1;

        /* divisors of 357 with Möbius values */
        const struct { unsigned d; int mu; } parts[] = {
            {1,   1},
            {3,  -1},
            {7,  -1},
            {17, -1},
            {21,  1},
            {51,  1},
            {119, 1},
            {357, -1}
        };
        const size_t nparts = sizeof(parts) / sizeof(parts[0]);

        for (size_t p = 0; p < nparts; ++p) {
            unsigned d = parts[p].d;
            int mu = parts[p].mu;

            if (mu == 1) {
                /* multiply by (1 - x^d) */
                for (i = 0; i < PERIOD; ++i) {
                    seq_t val = g[i];
                    if (i >= d) val -= g[i - d];
                    h[i] = val;
                }
            } else { /* mu == -1, multiply by 1/(1 - x^d) = 1 + x^d + x^{2d} + … */
                for (i = 0; i < PERIOD; ++i) h[i] = (seq_t)0;
                for (unsigned r = 0; r < d; ++r) {
                    seq_t sum = (seq_t)0;
                    for (unsigned k = 0; r + k * d < PERIOD; ++k) {
                        sum += g[r + k * d];
                        h[r + k * d] = sum;
                    }
                }
            }

            /* copy result back to g for next factor */
            for (i = 0; i < PERIOD; ++i) g[i] = h[i];
        }

        /* cache the computed period */
        for (i = 0; i < PERIOD; ++i) cache[i] = g[i];
        ready = 1;
    }

    return cache[n % PERIOD];
}
