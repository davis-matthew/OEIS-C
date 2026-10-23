/*
 * A014712  Inverse of 703rd cyclotomic polynomial.
 * https://oeis.org/A014712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the series 1/Φ₇₀₃(x), where
 *            Φ₇₀₃(x) is the 703rd cyclotomic polynomial. The sequence is
 *            periodic with period 703
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t period[703];
    static seq_t Q[649];           /* coefficients of Φ₇₀₃(x), Q[0] = 1 */

    if (!initialized) {
        /* Polynomial N(x) = (x^703 - 1)*(x - 1) */
        seq_t N[705] = {0};
        N[0]  =  1;    /* (-1)*(-1) */
        N[1]  = -1;    /* (-1)*x   */
        N[703] = -1;   /* x^703*(-1) */
        N[704] =  1;   /* x^703*x   */

        /* Polynomial D(x) = (x^37 - 1)*(x^19 - 1) */
        seq_t D[57] = {0};
        D[0]  =  1;    /* 1*1          */
        D[19] = -1;    /* 1*(-1)       */
        D[37] = -1;    /* (-1)*1       */
        D[56] =  1;    /* (-1)*(-1)    */

        /* Long division: N(x) = D(x) * Q(x) */
        for (int deg = 704; deg >= 56; --deg) {
            seq_t coeff = N[deg];
            if (coeff != 0) {
                int shift = deg - 56;                /* index in Q */
                Q[shift] = coeff;
                for (int k = 0; k <= 56; ++k) {
                    N[shift + k] -= coeff * D[k];
                }
            }
        }
        /* Q[0] should now be 1 (constant term of Φ₇₀₃) */

        /* Compute the series coefficients a(n) up to n = 702 using the recurrence:
           a[0] = 1
           a[n] = - Σ_{i=1}^{min(n,648)} Q[i] * a[n-i]
        */
        period[0] = 1;
        for (int i = 1; i < 703; ++i) {
            seq_t sum = 0;
            int lim = (i < 648) ? i : 648;
            for (int j = 1; j <= lim; ++j) {
                sum += Q[j] * period[i - j];
            }
            period[i] = -sum;
        }

        initialized = true;
    }

    /* Return the periodic value */
    size_t idx = (size_t)(n % 703);
    return period[idx];
}
