/*
 * A014568  Inverse of 559th cyclotomic polynomial.
 * https://oeis.org/A014568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the power‑series expansion of
 *            1/Φ_559(x), where Φ_559 is the 559th cyclotomic polynomial. The
 *            sequence is periodic with period 559
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t coeff[559];   /* a(0)…a(558) */
    static seq_t phi[505];     /* Φ_559(x)=∑phi[i]x^i, 0≤i≤504 */

    if (!ready) {
        /* ------------------------------------------------------------------
           1. Compute Φ_559(x) = (x^559-1)(x-1) / ((x^43-1)(x^13-1))
              using polynomial long division.
        ------------------------------------------------------------------ */
        static seq_t P[561] = {0};
        static seq_t Q[57]  = {0};
        static seq_t R[561] = {0};

        /* Numerator P(x) = (x^559-1)(x-1) = x^560 - x^559 - x + 1 */
        P[0]   = 1;        /* +1  */
        P[1]   = -1;       /* -x  */
        P[559] = -1;       /* -x^559 */
        P[560] = 1;        /* +x^560 */

        /* Denominator Q(x) = (x^43-1)(x^13-1) = x^56 - x^43 - x^13 + 1 */
        Q[0]   = 1;        /* +1  */
        Q[13]  = -1;       /* -x^13 */
        Q[43]  = -1;       /* -x^43 */
        Q[56]  = 1;        /* +x^56 */

        /* Copy P into the working remainder R */
        for (size_t i = 0; i < 561; ++i) R[i] = P[i];

        /* Long division: R(x) / Q(x)  →  phi(x)  */
        for (int i = 560; i >= 56; --i) {
            seq_t tc = R[i];          /* coefficient of x^i in the remainder */
            if (tc == 0) continue;    /* no term to cancel */

            int shift = i - 56;        /* degree of the current quotient term */

            phi[shift] = tc;                 /* store quotient coefficient */

            /* Subtract tc * x^shift * Q(x) from R */
            for (int j = 0; j <= 56; ++j) {
                R[shift + j] -= tc * Q[j];
            }
        }
        /* After division, phi[0..504] now holds Φ_559(x) coefficients.
           Remaining slots are already zero. */

        /* ------------------------------------------------------------------
           2. Compute the inverse series coefficients inv(k) for 0≤k<559
              using the recurrence:
                 inv[0] = 1
                 inv[k] = - ∑_{i=1}^{min(k,504)} phi[i] * inv[k-i]
        ------------------------------------------------------------------ */
        coeff[0] = 1;
        for (size_t k = 1; k < 559; ++k) {
            seq_t sum = 0;
            size_t max_i = (k < 504) ? k : 504;
            for (size_t i = 1; i <= max_i; ++i) {
                sum += phi[i] * coeff[k - i];
            }
            coeff[k] = -sum;
        }

        ready = true;
    }

    /* Periodic repetition with period 559 */
    return coeff[n % 559];
}
