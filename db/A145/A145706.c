/*
 * A145706  Expansion of chi(-x^5) / chi(-x^2) in powers of x where chi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A145706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k>0} (1 - x^{10k-5})/(1 -
 *            x^{4k-2})
 * Range:     i8 n <= 45, i16 n <= 127, i32 n <= 390, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate array f[0..n] and initialize to zero */
    size_t len = (size_t)(n + 1);
    seq_t *f = (seq_t *)calloc(len, sizeof(seq_t));
    if (!f) return 0;            /* Allocation failed; return 0 as safe fallback */
    f[0] = 1;                    /* Constant term of the product */

    /* Process each exponent j up to n */
    for (idx_t j = 1; j <= n; ++j) {
        /* Numerator factor (1 - x^j) when j ≡ 5 (mod 10) */
        if ((j % 10) == 5) {
            /* in-place subtraction: high-to-low to avoid using updated values */
            for (idx_t m = n; ; --m) {
                f[m] -= f[m - j];
                if (m == j) break;   /* Stop once m reaches j */
            }
        }
        /* Denominator factor (1 - x^j)^(-1) when j ≡ 2 (mod 4) */
        else if ((j % 4) == 2) {
            /* in-place addition: low-to-high, using the just-updated
               values of f[m - j] (which are new_f[m - j]) as required by
               the recurrence new_f[m] = g[m] + new_f[m - j] */
            for (idx_t m = j; m <= n; ++m) {
                f[m] += f[m - j];
            }
        }
    }

    seq_t result = f[n];
    free(f);
    return result;
}
