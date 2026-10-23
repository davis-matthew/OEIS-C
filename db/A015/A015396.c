/*
 * A015396  Inverse of 1387th cyclotomic polynomial.
 * https://oeis.org/A015396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = coefficient of x^n in the inverse power series of the
 *            1387‑th cyclotomic polynomial Φ1387(x). The sequence is periodic
 *            with period 1387
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t period[1387];
    static bool period_computed = false;

    if (!period_computed) {
        /* Parameters for the computation */
        const idx_t P1     = 19;   /* prime factor */
        const idx_t P2     = 73;   /* prime factor */
        const idx_t DEG_P1 = P1 - 1;                 /* 18   */
        const idx_t NUM_DEG = P2 * DEG_P1;           /* 1314 */
        const idx_t DIV_DEG = DEG_P1;                /* 18   */

        /* Build the numerator A(x) = Φ19(x^73) = Σ_{k=0}^{18} x^{73k}. */
        seq_t A[NUM_DEG + 1];
        for (idx_t i = 0; i <= NUM_DEG; ++i) A[i] = 0;
        for (idx_t k = 0; k <= DEG_P1; ++k) A[P2 * k] = 1;

        /* Build the divisor B(x) = Φ19(x) = Σ_{i=0}^{18} x^i. */
        seq_t B[DIV_DEG + 1];
        for (idx_t i = 0; i <= DIV_DEG; ++i) B[i] = 1;

        /* Polynomial long division: Q = A / B.  Q will be Φ1387(x). */
        seq_t Q[NUM_DEG - DIV_DEG + 1];
        for (idx_t i = 0; i <= NUM_DEG - DIV_DEG; ++i) Q[i] = 0;
        seq_t R[NUM_DEG + 1];
        for (idx_t i = 0; i <= NUM_DEG; ++i) R[i] = A[i];

        for (idx_t i = NUM_DEG; i >= DIV_DEG; --i) {
            seq_t coeff = R[i];
            if (coeff != 0) {
                Q[i - DIV_DEG] = coeff;
                for (idx_t j = 0; j <= DIV_DEG; ++j)
                    R[i - DIV_DEG + j] -= coeff;   /* B[j] == 1 */
            }
        }

        /* Compute the inverse series up to one period (1387 terms). */
        seq_t seq[1387];
        seq[0] = 1;
        for (idx_t m = 1; m < 1387; ++m) {
            seq_t sum = 0;
            idx_t maxk = (m <= 1296) ? m : 1296;   /* use φ coefficients 1..1296 */
            for (idx_t k = 1; k <= maxk; ++k)
                sum += Q[k] * seq[m - k];
            seq[m] = -sum;
        }

        /* Store the period for fast subsequent access. */
        for (idx_t i = 0; i < 1387; ++i) period[i] = seq[i];
        period_computed = true;
    }

    return period[n % 1387];
}
