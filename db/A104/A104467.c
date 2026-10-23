/*
 * A104467  Coefficients of the A-Bailey Mod 9 identity.
 * https://oeis.org/A104467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   product_{k>0} (1 - q^{9k-4})(1 - q^{9k-5}) / ((1 - q^{9k-3})(1 -
 *            q^{9k-6}))
 * Range:     i8 n <= 41, i16 n <= 122, i32 n <= 383, i64 n <= 1301,
 *            i128 n <= 4697, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *coeff = NULL;
static idx_t max_cached = 0;

static void recompute_to(idx_t n)
{
    if (coeff) free(coeff);
    coeff = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!coeff) abort();               /* allocation failed */
    for (idx_t i = 0; i <= n; ++i) coeff[i] = 0;
    coeff[0] = 1;                      /* constant term */

    for (idx_t k = 1;; ++k) {
        idx_t m1 = 9 * k - 4;
        idx_t m2 = 9 * k - 5;
        idx_t m3 = 9 * k - 3;
        idx_t m4 = 9 * k - 6;

        if (m1 > n && m2 > n && m3 > n && m4 > n) break;

        if (m1 <= n) {
            for (idx_t i = n; i >= m1; --i) coeff[i] -= coeff[i - m1];
        }
        if (m2 <= n) {
            for (idx_t i = n; i >= m2; --i) coeff[i] -= coeff[i - m2];
        }
        if (m3 <= n) {
            for (idx_t i = m3; i <= n; ++i) coeff[i] += coeff[i - m3];
        }
        if (m4 <= n) {
            for (idx_t i = m4; i <= n; ++i) coeff[i] += coeff[i - m4];
        }
    }
    max_cached = n;
}

seq_t a(idx_t n)
{
    if (!coeff || n > max_cached) recompute_to(n);
    return coeff[n];
}
