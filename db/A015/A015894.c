/*
 * A015894  Inverse of 1885th cyclotomic polynomial.
 * https://oeis.org/A015894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the expansion of 1/Φ_{1885}(x)
 *            (period 1885)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* seq_t and idx_t are defined in the build. */

seq_t a(idx_t n)
/* Inverse of 1885th cyclotomic polynomial (periodic, period 1885) */
{
    static seq_t cache[1885];
    static bool initialized = false;

    if (!initialized) {
        const idx_t period = 1885;
        seq_t cur[1885] = {0};
        seq_t next[1885];

        /* start with the series 1 */
        cur[0] = 1;

        /* multiply by (x^1 - 1) */
        for (idx_t k = 0; k < period; k++) {
            next[k] = (k >= 1 ? cur[k - 1] : (seq_t)0) - cur[k];
        }
        for (idx_t k = 0; k < period; k++) cur[k] = next[k];

        /* multiply by -1/(1 - x^d) for d = 5, 13, 29 */
        const idx_t invs[3] = {5, 13, 29};
        for (int ii = 0; ii < 3; ii++) {
            idx_t d = invs[ii];
            for (idx_t r = 0; r < d; r++) {
                seq_t cum = 0;
                for (idx_t k = r; k < period; k += d) {
                    cum += cur[k];
                    next[k] = -cum;
                }
            }
            for (idx_t k = 0; k < period; k++) cur[k] = next[k];
        }

        /* global sign flip from the factor 1/(x^1885 - 1) */
        for (idx_t k = 0; k < period; k++) {
            cur[k] = -cur[k];
        }

        /* multiply by (x^d - 1) for d = 65, 145, 377 */
        const idx_t poss[3] = {65, 145, 377};
        for (int ii = 0; ii < 3; ii++) {
            idx_t d = poss[ii];
            for (idx_t k = 0; k < period; k++) {
                next[k] = (k >= d ? cur[k - d] : (seq_t)0) - cur[k];
            }
            for (idx_t k = 0; k < period; k++) cur[k] = next[k];
        }

        /* store the final coefficients in the cache */
        for (idx_t k = 0; k < period; k++) {
            cache[k] = cur[k];
        }
        initialized = true;
    }

    return cache[n % 1885];
}
