/*
 * A014086  Inverse of 77th cyclotomic polynomial.
 * https://oeis.org/A014086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = - Σ_{k=1}^{min(n,60)} c_k · a(n-k) (a(0)=1) where c_k are
 *            the coefficients of the 77‑th cyclotomic polynomial Φ₇₇(x)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The coefficients of Φ₇₇(x) are computed once and stored. */
    static bool init = false;
    static seq_t period[77];

    if (!init)
    {
        /* 1. Build P(x) = (x^77 - 1)(x - 1) = x^78 - x^77 - x + 1  */
        seq_t P[79] = {0};          /* indices 0 … 78 */
        P[78] = 1;
        P[77] = -1;
        P[1]  = -1;
        P[0]  = 1;

        /* 2. Build Q(x) = (x^11 - 1)(x^7 - 1) = x^18 - x^11 - x^7 + 1 */
        seq_t Q[19] = {0};          /* indices 0 … 18 */
        Q[18] = 1;
        Q[11] = -1;
        Q[7]  = -1;
        Q[0]  = 1;

        /* 3. Long division: P(x) / Q(x) = Φ₇₇(x)  (degree 60) */
        seq_t temp[79];
        for (idx_t i = 0; i < 79; ++i) temp[i] = P[i];

        seq_t phi_coeff[61] = {0};  /* indices 0 … 60 */
        const int degQ = 18;
        for (int s = 60; s >= 0; --s)          /* quotient degree s */
        {
            seq_t coef = temp[s + degQ];        /* leading term of the current remainder */
            phi_coeff[s] = coef;
            for (int k = 0; k <= degQ; ++k)
                temp[s + k] -= coef * Q[k];
        }

        /* 4. Recurrence for the coefficients of 1/Φ₇₇(x)  */
        period[0] = 1;                         /* a(0) */
        for (idx_t m = 1; m < 77; ++m)
        {
            seq_t sum = 0;
            idx_t kmax = (m < 60 ? m : 60);
            for (idx_t k = 1; k <= kmax; ++k)
                sum += phi_coeff[k] * period[m - k];
            period[m] = -sum;
        }

        init = true;
    }

    return period[n % 77ULL];
}
