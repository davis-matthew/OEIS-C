/*
 * A014536  Inverse of 527th cyclotomic polynomial.
 * https://oeis.org/A014536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A014536: coefficients of 1/Φ₅₂₇(x), periodic with period 527.  */
/* The algorithm first builds the cyclotomic polynomial Φ₅₂₇(x) by dividing */
/* x^527−1 by (x−1)·Φ₁₇(x)·Φ₃₁(x).  The coefficients of this divisor are */
/* easy to form because Φ₁₇(x)=Φ₃₁(x)=1+x+⋯+x^{p−1}.  Division gives the   */
/* coefficients C[0..480] of Φ₅₂₇(x) (C[0]=1).  Then the inverse power   */
/* series a(n) satisfies a(0)=1 and a(n) = −∑_{k=1}^{480} C[k]·a(n−k).  */
/* The sequence is periodic with a( n+527 ) = a(n). */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t period[527];

    if (!initialized) {
        /* (x−1)·Φ₁₇(x)·Φ₃₁(x)  -- 48 coefficients */
        const int L17 = 17;
        const int L31 = 31;
        const int Lt  = L17 + L31 - 1;   /* 47   */
        const int Qlen = Lt + 1;         /* 48   */

        int32_t temp[Lt];
        int32_t Q[Qlen];

        for (int i = 0; i < Lt; ++i) temp[i] = 0;
        for (int i = 0; i < L17; ++i)
            for (int j = 0; j < L31; ++j)
                temp[i + j] += 1;          /* Φ₁₇ and Φ₃₁ are all 1's */

        for (int i = 0; i < Qlen; ++i) Q[i] = 0;
        for (int i = 0; i < Lt; ++i) {
            Q[i]   += -temp[i];           /* multiply by (x−1) */
            Q[i+1] +=  temp[i];
        }

        /* Divide x^527−1 by Q to obtain Φ₅₂₇(x) coefficients C[0..480] */
        const int Rlen = 480 + 1;
        int32_t C[Rlen];

        for (int k = 0; k < Rlen; ++k) {
            int32_t sum = 0;
            int max_i = (k < 47) ? k : 47;
            for (int i = 1; i <= max_i; ++i)
                sum += Q[i] * C[k - i];

            int32_t N = 0;
            if (k == 0) N = -1;        /* N[0] = -1 in x^527−1 */
            /* N[527] = 1 does not affect C[0..480] computation */

            C[k] = sum - N;            /* because Q[0] = -1 */
        }

        /* Compute inverse series a[0..526] using recurrence */
        period[0] = 1;
        for (idx_t m = 1; m < 527; ++m) {
            seq_t sum = 0;
            int max_i = (m <= 480) ? (int)m : 480;
            for (int i = 1; i <= max_i; ++i)
                sum += (seq_t)C[i] * period[m - i];

            period[m] = -sum;
        }

        initialized = true;
    }

    return period[n % 527];
}
