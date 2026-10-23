/*
 * A015514  Inverse of 1505th cyclotomic polynomial.
 * https://oeis.org/A015514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) is the coefficient of x^n in the formal power series
 *            inverse of the 1505th cyclotomic polynomial Φ₁₅₀₅(x). The
 *            sequence is periodic with period 1505
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   compose_poly
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/compose_poly.h"

/* exact division of polynomials (numerator / denominator), denominator is monic */
static seq_t *poly_div_exact(const seq_t *num, size_t numSize,
                             const seq_t *den, size_t denSize,
                             size_t *outSize) {
    size_t n = numSize - 1;
    size_t m = denSize - 1;
    size_t qSize = (n >= m) ? (n - m + 1) : 0;
    seq_t *Q = (seq_t*)malloc(qSize * sizeof(seq_t));
    for (size_t i = 0; i < qSize; ++i) Q[i] = 0;
    seq_t *R = (seq_t*)malloc(numSize * sizeof(seq_t));
    for (size_t i = 0; i < numSize; ++i) R[i] = num[i];

    for (int i = (int)n; i >= (int)m; --i) {
        seq_t coeff = R[i];
        Q[i - m] = coeff;
        if (coeff) {
            for (int j = 0; j <= (int)m; ++j) {
                R[i - m + j] -= coeff * den[j];
            }
        }
    }
    free(R);
    *outSize = qSize;
    return Q;
}

seq_t a(idx_t n) {
    static seq_t *period_values = NULL;
    const size_t period = 1505;

    if (!period_values) {
        /* compute Φ_1505(x) coefficients */
        size_t polySize = 5;                     /* coefficients of Φ₅(x) */
        seq_t *poly = (seq_t*)malloc(polySize * sizeof(seq_t));
        for (size_t i = 0; i < polySize; ++i) poly[i] = 1;   /* 1 + x + … + x⁴ */

        /* prime factors of 1505: 5, 7, 43 – already used 5, now apply 7 and 43 */
        int primes[2] = {7, 43};
        for (int p_idx = 0; p_idx < 2; ++p_idx) {
            int p = primes[p_idx];
            size_t tempSize;
            seq_t *temp = compose_poly(poly, polySize, p, &tempSize);
            size_t newSize;
            seq_t *newPoly = poly_div_exact(temp, tempSize, poly, polySize, &newSize);
            free(poly);
            free(temp);
            poly = newPoly;
            polySize = newSize;
        }
        /* now poly holds Φ₁₅₀₅(x) coefficients, degree = polySize‑1 = 1008 */

        /* compute sequence a[0..period‑1] via recurrence with these coefficients */
        period_values = (seq_t*)malloc(period * sizeof(seq_t));
        period_values[0] = 1;
        size_t phiDeg = polySize - 1;
        for (size_t k = 1; k < period; ++k) {
            seq_t sum = 0;
            size_t lim = (k < phiDeg) ? k : phiDeg;
            for (size_t i = 1; i <= lim; ++i) {
                sum += poly[i] * period_values[k - i];
            }
            period_values[k] = -sum;
        }
        free(poly);
    }

    size_t idx = (size_t)(n % period);
    return period_values[idx];
}
