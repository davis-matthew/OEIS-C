/*
 * A230322  Expansion of f(-x^3, -x^4) / f(-x^2,-x^5) in powers of x where
 *          f(,) is Ramanujan's two-variable theta function.
 * https://oeis.org/A230322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^n] ∏_{k≥1} (1 - x^(7k-3)) (1 - x^(7k-4)) / ((1 -
 *            x^(7k-2)) (1 - x^(7k-5)))
 * Range:     i8 n <= 97, i16 n <= 266, i32 n <= 826, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* allocate coefficient array a[0..n] */
    idx_t len = n + 1;
    seq_t *coeff = (seq_t *)malloc((size_t)len * sizeof(seq_t));
    if (!coeff) return (seq_t)0;          /* out of memory – return 0 */

    /* initialise: coeff[0] = 1, others 0 */
    coeff[0] = 1;
    for (idx_t i = 1; i < len; ++i) coeff[i] = 0;

    /* periodic exponents e_m for m = 1..n:
       e_m = +1 for m≡3,4 (numerator), -1 for m≡2,5 (denominator),
       0 otherwise. */
    static const int es[7] = {0, -1, 1, 1, -1, 0, 0};

    for (idx_t m = 1; m <= n; ++m) {
        int e = es[(m - 1) % 7];
        if (e == 0) continue;

        if (e > 0) { /* multiply by (1 - x^m) */
            for (idx_t i = n; i >= m; --i)
                coeff[i] -= coeff[i - m];
        } else {     /* divide by (1 - x^m) */
            for (idx_t i = m; i <= n; ++i)
                coeff[i] += coeff[i - m];
        }
    }

    seq_t result = coeff[n];
    free(coeff);
    return result;
}
