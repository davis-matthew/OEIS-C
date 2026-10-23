/*
 * A014502  Inverse of 493rd cyclotomic polynomial.
 * https://oeis.org/A014502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is coefficient of x^n in the formal power series expansion
 *            of 1 / Φ_493(x), where Φ_493(x) is the 493rd cyclotomic
 *            polynomial. The sequence is periodic with period 493
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static buffers for one-time initialization */
    static bool init = false;
    static seq_t p[449];        /* Φ_493 coefficients (degree 448) */
    static seq_t a_arr[493];    /* first 493 terms of the sequence */

    if (!init) {
        /* ---------------------------------------------
           Compute Φ_493(x) by polynomial division
           Φ_493(x) = (x^493 - 1) /
                      ((x - 1) * (x^17 - 1)/(x - 1) * (x^29 - 1)/(x - 1))
           i.e. (x^493 - 1) / ((x-1)*(1+x+...+x^16)*(1+x+...+x^28))
        ------------------------------------------------ */

        /* polynomial arrays: indices = exponent */
        seq_t numerator[494] = {0};  /* x^493 - 1 */
        numerator[0]  = (seq_t)-1;
        numerator[493] = (seq_t)1;

        /* (x - 1) */
        seq_t fac1[2] = {(seq_t)-1, (seq_t)1};

        /* 1 + x + ... + x^16  (Φ_17) */
        seq_t fac17[17];
        for (idx_t i = 0; i < 17; ++i) fac17[i] = (seq_t)1;

        /* 1 + x + ... + x^28  (Φ_29) */
        seq_t fac29[29];
        for (idx_t i = 0; i < 29; ++i) fac29[i] = (seq_t)1;

        /* multiply fac1 and fac17 -> temp (x^17 - 1) */
        seq_t temp1[18] = {0};
        for (idx_t i = 0; i < 2; ++i)
            for (idx_t j = 0; j < 17; ++j)
                temp1[i + j] += fac1[i] * fac17[j];   /* exact because small */

        /* multiply temp1 (degree 17) with fac29 -> denom */
        seq_t denom[46] = {0};
        for (idx_t i = 0; i < 18; ++i)
            for (idx_t j = 0; j < 29; ++j)
                denom[i + j] += temp1[i] * fac29[j];   /* exact */

        /* denominator has degree 45 */
        const idx_t ddeg = 45;   /* 17 + 28 - 1 */

        /* division: numerator (degree 493) / denom (degree 45) -> quotient p */
        seq_t quot[449] = {0};    /* degree 448 */

        for (idx_t k = 493; k >= ddeg; --k) {
            seq_t q = numerator[k];
            if (q != (seq_t)0) {
                quot[k - ddeg] = q;
                /* subtract q * denom shifted by (k - ddeg) */
                for (idx_t i = 0; i <= ddeg; ++i) {
                    numerator[k - ddeg + i] -= q * denom[i];
                }
            }
        }

        /* after division, low part of numerator should be zero */
        for (idx_t i = 0; i <= ddeg; ++i)
            numerator[i] = (seq_t)0;

        /* copy quotient to Φ_493 coefficients */
        for (idx_t i = 0; i <= 448; ++i)
            p[i] = quot[i];

        /* ---------------------------------------------
           Compute first 493 terms of the reciprocal series
           Using recurrence : a[0]=1,
           a[n] = -sum_{j=1}^{min(n, deg)} p[j]*a[n-j]
        ------------------------------------------------ */
        a_arr[0] = (seq_t)1;
        for (idx_t m = 1; m < 493; ++m) {
            seq_t sum = (seq_t)0;
            idx_t up = (m < 449) ? m : 448;
            for (idx_t j = 1; j <= up; ++j) {
                sum += p[j] * a_arr[m - j];
            }
            a_arr[m] = -sum;
        }

        init = true;
    }

    /* sequence has period 493 */
    idx_t idx = n % 493;
    return a_arr[idx];
}
