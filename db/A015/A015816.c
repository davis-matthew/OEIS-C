/*
 * A015816  Inverse of 1807th cyclotomic polynomial.
 * https://oeis.org/A015816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the formal power series * 1 /
 *            Φ_1807(x) where Φ_1807(x) is the 1807‑th cyclotomic *
 *            polynomial. The sequence is periodic with period 1807 and * the
 *            coefficients satisfy the recurrence * * a(0) = 1 * a(n) =
 *            -∑_{k=1}^{φ(1807)} c_k · a(n-k) (n ≥ 1) * * where the c_k...
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*  φ(1807) = (13‑1)(139‑1) = 12·138 = 1656  */
#define DEG    1656
/*  Period length */
#define PERIOD 1807

seq_t a(idx_t n)
{
    static bool init = false;
    static int8_t  phi_coeff[DEG + 1];   /* coefficients of Φ_1807 */
    static seq_t  period_seq[PERIOD];    /* a[0..PERIOD-1] */

    if (!init) {
        /* --- compute Φ_1807 coefficients --------------------------------- */
        /* N(x) = (x-1)(x^1807-1) has non‑zero coefficients only at
         * degree 0 (+1), degree 1 (‑1), degree 1807 (‑1) and degree 1808 (+1).
         * We only need N[i] for i ≤ DEG = 1656, so
         *    N[0] =  1 , N[1] = -1 , N[i] = 0 otherwise.            */
        for (int i = 0; i <= DEG; ++i) {
            int val = 0;
            if (i == 0)           val =  1;
            else if (i == 1)      val = -1;
            /* D(x) = (x^13-1)(x^139-1) has coefficients:
             * D(0)=+1, D(13)=-1, D(139)=-1, D(152)=+1.
             * Because D[0]=1 we can recover the quotient Q(x) by
             *   Q[i] = N[i] + Q[i-13] + Q[i-139] - Q[i-152]   (i ≥ 0)
             * (indices < 0 are ignored). */
            if (i >= 13)          val += phi_coeff[i - 13];
            if (i >= 139)         val += phi_coeff[i - 139];
            if (i >= 152)         val -= phi_coeff[i - 152];
            phi_coeff[i] = (int8_t)val;
        }

        /* --- compute first PERIOD coefficients of 1/Φ_1807 ---------------- */
        period_seq[0] = 1;
        for (int i = 1; i < PERIOD; ++i) {
            seq_t val = 0;
            int maxk = (i <= DEG) ? i : DEG;
            for (int k = 1; k <= maxk; ++k) {
                val -= (seq_t)phi_coeff[k] * period_seq[i - k];
            }
            period_seq[i] = val;
        }

        init = true;
    }

    idx_t idx = n % PERIOD;
    return period_seq[idx];
}
