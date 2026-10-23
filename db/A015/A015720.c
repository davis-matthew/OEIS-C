/*
 * A015720  Inverse of 1711th cyclotomic polynomial.
 * https://oeis.org/A015720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) is the coefficient of x^n in the power series expansion of
 *            1/Φ_1711(x). For Φ_1711 the inverse series satisfies the
 *            identity (1 − x) · (1 − x^29) · (1 − x^59) · (1 − x^1711) ·
 *            1/Φ_1711(x) = 1 Hence 1/Φ_1711(x) can be obtained by multiplying
 *            the series 1 successively by the factors 1/(1−x^171...
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* allocate array for coefficients up to n */
    seq_t *coeff = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!coeff) return 0;          /* allocation failed – undefined behaviour */
    coeff[0] = (seq_t)1;           /* a(0) = 1 */

    /* 1/(1 - x^1711) factor */
    idx_t k = 1711;
    for (idx_t i = k; i <= n; ++i) coeff[i] += coeff[i - k];

    /* (1 - x^59) factor */
    k = 59;
    for (idx_t i = n; i >= k; --i) coeff[i] -= coeff[i - k];
    /* (1 - x^29) factor */
    k = 29;
    for (idx_t i = n; i >= k; --i) coeff[i] -= coeff[i - k];

    /* 1/(1 - x) factor */
    k = 1;
    for (idx_t i = 1; i <= n; ++i) coeff[i] += coeff[i - 1];

    seq_t result = coeff[n];
    free(coeff);
    return result;
}
