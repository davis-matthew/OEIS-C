/*
 * A014124  Inverse of 115th cyclotomic polynomial.
 * https://oeis.org/A014124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) are the coefficients of the power series expansion of 1 /
 *            Φ₁₁₅(x) (the 115th cyclotomic polynomial). The sequence is
 *            periodic with period 115
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Period of the sequence */
#define PERIOD 115

seq_t a(idx_t n)
{
    /* Tables for the cyclotomic polynomial Φ₁₁₅(x) and the first period
       of its reciprocal series.  They are filled once on first call.
    */
    static seq_t  phi_coef[89];   /* coefficients of Φ₁₁₅(x), degree 88 */
    static seq_t  period[PERIOD]; /* first PERIOD terms of a(n) */
    static bool   ready = false;

    if (!ready) {
        /* ----------------------------------------------------------------
           Compute Φ₁₁₅(x) by polynomial long division:
               Φ₁₁₅(x) = (x¹¹⁶ - x¹¹⁵ - x + 1) / (x²⁸ - x²³ - x⁵ + 1)
         ---------------------------------------------------------------- */
        seq_t num[117] = {0}; /* numerator: (x^115-1)(x-1) */
        seq_t den[29]  = {0}; /* denominator: (x^23-1)(x^5-1) */

        /* Numerator coefficients */
        num[116] = 1;   /* x^116  */
        num[115] = -1;  /* -x^115 */
        num[1]   = -1;  /* -x     */
        num[0]   = 1;   /* +1     */

        /* Denominator coefficients */
        den[28] = 1;   /* x^28  */
        den[23] = -1;  /* -x^23 */
        den[5]  = -1;  /* -x^5  */
        den[0]  = 1;   /* +1    */

        /* Long division of num by den to get Φ₁₁₅(x) */
        seq_t q[89] = {0};
        for (int i = 116; i >= 28; --i) {
            seq_t coeff = num[i];
            if (coeff) {
                q[i - 28] = coeff;        /* leading term of quotient */
                for (int j = 0; j <= 28; ++j) {
                    num[i - 28 + j] -= coeff * den[j];
                }
            }
        }
        /* Copy into phi_coef array (degree 0 .. 88) */
        for (int i = 0; i <= 88; ++i)
            phi_coef[i] = q[i];

        /* ----------------------------------------------------------------
           Compute the first PERIOD terms of the reciprocal series
           using the recurrence:
               a(0) = 1
               a(n) = -Σ_{k=1}^{min(n,88)} φ(k) * a(n-k)
           ---------------------------------------------------------------- */
        period[0] = 1;
        for (int idx = 1; idx < PERIOD; ++idx) {
            seq_t sum = 0;
            int maxk = (idx <= 88) ? idx : 88;
            for (int k = 1; k <= maxk; ++k) {
                sum += phi_coef[k] * period[idx - k];
            }
            period[idx] = -sum;
        }

        ready = true;
    }

    /* Return the requested term using periodicity */
    size_t pos = (size_t)(n % PERIOD);
    return period[pos];
}
