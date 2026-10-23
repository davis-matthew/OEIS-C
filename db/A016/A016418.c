/*
 * A016418  473rd cyclotomic polynomial.
 * https://oeis.org/A016418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   Φ_473(x) = (x^473 - 1)(x - 1) / ((x^43 - 1)(x^11 - 1))
 * Range:     i8 n <= 420, i16 n <= 420, i32 n <= 420, i64 n <= 420,
 *            i128 n <= 420, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Coefficients are computed once and cached. */
seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t coeff[421];   /* degree φ(473) = 420, indices 0..420 */

    if (!init) {
        /* Fixed-size polynomial arrays: degrees 474 and 54 respectively. */
        seq_t N[475] = {0};
        seq_t D[55]  = {0};

        /* N(x) = (x^473 - 1)(x - 1) = x^474 - x^473 - x + 1 */
        N[0]   = (seq_t) 1;
        N[1]   = (seq_t) -1;
        N[473] = (seq_t) -1;
        N[474] = (seq_t)  1;

        /* D(x) = (x^43 - 1)(x^11 - 1) = x^54 - x^43 - x^11 + 1 */
        D[0]   = (seq_t)  1;
        D[11]  = (seq_t) -1;
        D[43]  = (seq_t) -1;
        D[54]  = (seq_t)  1;

        /* Compute the quotient Q = N / D via long division. */
        for (int64_t idx = 474; idx >= 54; --idx) {
            seq_t c = N[(size_t)idx];
            if (c == (seq_t)0) continue;
            size_t qpos = (size_t)(idx - 54);
            coeff[qpos] = c;

            /* Subtract c * D(x) shifted by qpos from N */
            N[qpos]        -= c * D[0];
            N[qpos + 11]   -= c * D[11];
            N[qpos + 43]   -= c * D[43];
            N[qpos + 54]   -= c * D[54];
        }

        init = true;
    }

    if (n <= (idx_t)420) {
        return coeff[(size_t)n];
    } else {
        return (seq_t)0;   /* coefficients beyond the degree are zero */
    }
}
