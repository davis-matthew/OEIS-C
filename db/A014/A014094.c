/*
 * A014094  Inverse of 85th cyclotomic polynomial.
 * https://oeis.org/A014094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power series 1/Φ_85(x), where
 *            Φ_85(x) is the 85th cyclotomic polynomial. The sequence is
 *            periodic with period length 85
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t phi[65];            /* the 85th cyclotomic coefficients */
    static seq_t period[85];         /* first 85 terms of the sequence */

    if (!initialized) {
        /* compute Φ_85(x) = Φ_17(x^5) / Φ_17(x) */
        static seq_t polyP[17];     /* Φ_17(x) = 1 + x + ... + x^16 */
        static seq_t polyQ[81];     /* Φ_17(x^5) coefficients */

        for (idx_t i = 0; i <= 16; ++i) polyP[i] = 1;

        for (idx_t k = 0; k <= 80; ++k)
            polyQ[k] = (k % 5 == 0 && k / 5 <= 16) ? 1 : 0;

        /* perform polynomial division polyQ / polyP,
           obtaining the quotient coefficients φ[0..64]   */
        for (idx_t i = 0; i <= 64; ++i) phi[i] = 0;

        for (idx_t m = 0; m <= 80; ++m) {
            seq_t sum = 0;
            for (idx_t i = 1; i <= 16 && i <= m; ++i) {
                idx_t idx = m - i;
                if (idx <= 64) sum += phi[idx];
            }
            seq_t q = polyQ[m];
            if (m <= 64)
                phi[m] = q - sum;
            /* otherwise we are in the remainder part, which must be zero */
        }

        /* compute the first 85 terms of the inverse series */
        period[0] = 1;
        for (idx_t k = 1; k < 85; ++k) {
            seq_t s = 0;
            for (idx_t i = 1; i <= 64 && i <= k; ++i)
                s += phi[i] * period[k - i];
            period[k] = -s;
        }

        initialized = true;
    }

    return period[n % 85];
}
