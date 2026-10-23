/*
 * A015712  Inverse of 1703rd cyclotomic polynomial.
 * https://oeis.org/A015712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) are the coefficients of the power series expansion of 1 /
 *            Φ_1703(x), where Φ_1703(x) is the 1703‑th cyclotomic polynomial.
 *            The sequence is periodic with period 1703; the coefficients are
 *            computed once by polynomial division
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* size of the period */
    const size_t period_len = 1703;
    /* degree of Φ_1703(x) */
    const size_t deg_phi = 1560;
    /* static state to compute the period only once */
    static seq_t *period = NULL;
    static seq_t *phi_coeff = NULL;
    static bool built = false;

    if (!built) {
        /* allocate storage */
        period    = (seq_t *)malloc(period_len * sizeof(seq_t));
        phi_coeff = (seq_t *)malloc((deg_phi + 1) * sizeof(seq_t));
        if (!period || !phi_coeff) {
            /* allocation failure – return zero for all n */
            if (period) free(period);
            if (phi_coeff) free(phi_coeff);
            return (seq_t)0;
        }

        /* ---------- polynomial division to get Φ_1703(x) ----------
         * Φ_1703(x) = (x^1703 - 1)(x - 1) / [(x^13 - 1)(x^131 - 1)]
         * Numerator:  P(x) = x^1704 - x^1703 - x + 1
         * Denominator: Q(x) = x^144 - x^131 - x^13 + 1
         */
        const size_t deg_P = 1704;
        const size_t deg_Q = 144;
        seq_t *P = (seq_t *)malloc((deg_P + 1) * sizeof(seq_t));
        seq_t Q[deg_Q + 1];
        if (!P) {
            free(period); free(phi_coeff);
            return (seq_t)0;
        }

        /* initialise P to zero */
        for (size_t i = 0; i <= deg_P; ++i) P[i] = 0;
        P[0] = 1;            /* constant term 1 */
        P[1] = -1;           /* coefficient of x    */
        P[1703] = -1;        /* coefficient of x^1703  */
        P[1704] = 1;         /* coefficient of x^1704 */

        /* initialise Q to zero */
        for (size_t i = 0; i <= deg_Q; ++i) Q[i] = 0;
        Q[0]   = 1;          /* constant term 1 */
        Q[13]  = -1;         /* coefficient of x^13  */
        Q[131] = -1;         /* coefficient of x^131 */
        Q[144] = 1;          /* coefficient of x^144 */

        /* long division: P(x) / Q(x) = Φ_1703(x) */
        for (size_t i = deg_P; i >= deg_Q; --i) {
            seq_t coeff = P[i];              /* Q's leading coeff is 1 */
            phi_coeff[i - deg_Q] = coeff;
            for (size_t k = 0; k <= deg_Q; ++k) {
                P[(i - deg_Q) + k] -= coeff * Q[k];
            }
        }

        /* ---------- compute the period coefficients ----------
         * Recurrence:  sum_{k=0}^{min(deg_phi,n)} phi_coeff[k] * a[n-k] = 0
         * for n >= 1, with a[0] = 1
         */
        period[0] = 1;
        for (size_t n = 1; n < period_len; ++n) {
            seq_t sum = 0;
            size_t maxk = (n < deg_phi) ? n : deg_phi;
            for (size_t k = 1; k <= maxk; ++k) {
                sum += phi_coeff[k] * period[n - k];
            }
            period[n] = -sum;
        }

        /* free temporary storage */
        free(P);

        /* mark as built */
        built = true;
    }

    /* return the periodic value */
    size_t idx = (size_t)(n % period_len);
    return period[idx];
}
