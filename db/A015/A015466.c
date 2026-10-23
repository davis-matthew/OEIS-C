/*
 * A015466  Inverse of 1457th cyclotomic polynomial.
 * https://oeis.org/A015466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = coefficient of x^n in the power series expansion of
 *            1/Φ_{1457}(x) * where Φ_{1457} is the 1457th cyclotomic
 *            polynomial. * The sequence is periodic with period 1457
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t coeff[1381];   /* Φ_{1457} coefficients 0..1380   */
    static seq_t period[1457];  /* one full period of the sequence */

    if (!ready) {
        /* ---- Step 1: compute Φ_{1457} coefficients by polynomial division ---- */
        const size_t DEG_DEN  = 78;   /* degree of denominator polynomial  */
        const size_t DEG_NUM  = 1458; /* degree of numerator polynomial   */
        const size_t QLEN     = DEG_NUM - DEG_DEN + 1; /* 1381 */

        seq_t D[DEG_DEN + 1];
        seq_t M[DEG_NUM + 1];

        /* zero all coefficients */
        for (size_t i = 0; i <= DEG_NUM; ++i) M[i] = (seq_t)0;
        for (size_t i = 0; i <= DEG_DEN; ++i) D[i] = (seq_t)0;

        /* Numerator: (x^{1458} - x^{1457} - x + 1) */
        M[0]          = (seq_t)1;
        M[1]          = (seq_t)-1;
        M[1457]       = (seq_t)-1;
        M[1458]       = (seq_t)1;

        /* Denominator: (x^{78} - x^{47} - x^{31} + 1) */
        D[0]          = (seq_t)1;
        D[31]         = (seq_t)-1;
        D[47]         = (seq_t)-1;
        D[78]         = (seq_t)1;

        /* Long division M / D → coeff (monic divisor) */
        for (int ii = (int)DEG_NUM; ii >= (int)DEG_DEN; --ii) {
            seq_t c = M[ii];
            if (c != (seq_t)0) {
                coeff[ii - DEG_DEN] = c;
                /* subtract c * D shifted by (ii - DEG_DEN) */
                for (int j = 0; j <= (int)DEG_DEN; ++j) {
                    M[(ii - DEG_DEN) + j] -= c * D[j];
                }
            }
        }

        /* ---- Step 2: generate the periodic sequence up to one full period ---- */
        const size_t PERIOD   = 1457;
        const size_t DEG_PHI  = DEG_NUM - DEG_DEN;   /* 1380    */

        period[0] = (seq_t)1;
        for (idx_t nn = 1; nn < PERIOD; ++nn) {
            seq_t sum = (seq_t)0;
            size_t max_i = (nn <= DEG_PHI) ? (size_t)nn : DEG_PHI;
            for (size_t i = 1; i <= max_i; ++i) {
                sum += coeff[i] * period[nn - i];
            }
            period[nn] = -sum;
        }

        ready = true;
    }

    /* Return the periodic value */
    return period[(size_t)(n % (idx_t)1457)];
}
