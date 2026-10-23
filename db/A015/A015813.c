/*
 * A015813  Inverse of 1804th cyclotomic polynomial.
 * https://oeis.org/A015813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series of coefficients of the inverse of the 1804‑th cyclotomic polynomial.
   a(n) = - coeff of x^{n mod 1804} in Q(x), where Q(x) = (x^1804 - 1) / Φ_1804(x).
   The sequence is periodic with period 1804.
*/
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t Qglobal[1805];

    if (!ready) {
        /* List of divisors of 1804 in ascending order */
        const idx_t divs[] = {1, 2, 4, 11, 22, 41, 44,
                              82, 164, 451, 902, 1804};
        const size_t num_divs = sizeof(divs) / sizeof(divs[0]);

        static seq_t phi_coeff[12][1805];
        static int phi_deg[12];
        /* ------------------------------------------------------------------ */
        /* Compute Φ_d for each divisor d of 1804 using division
           Φ_d(x) = (x^d - 1) / ∏_{k|d, k<d} Φ_k(x)                          */
        for (size_t idx = 0; idx < num_divs; ++idx) {
            idx_t d = divs[idx];
            if (d == 1) {                      /* Φ_1(x) = x - 1 */
                phi_deg[idx] = 1;
                phi_coeff[idx][0] = -1;
                phi_coeff[idx][1] = 1;
                continue;
            }

            /* Build product P(x) = ∏_{k|d, k<d} Φ_k(x) */
            seq_t P[1805] = {0};
            P[0] = 1;          /* constant 1 */
            size_t P_len = 1;   /* length of P */

            for (size_t j = 0; j < idx; ++j) {       /* earlier divisors */
                if (d % divs[j] != 0) continue;
                size_t degDiv = phi_deg[j];
                seq_t TMP[1805] = {0};

                /* multiply P by Φ_{divs[j]} */
                for (size_t i = 0; i < P_len; ++i) {
                    if (P[i] == 0) continue;
                    for (size_t k = 0; k <= degDiv; ++k) {
                        if (phi_coeff[j][k] == 0) continue;
                        TMP[i + k] += P[i] * phi_coeff[j][k];
                    }
                }
                size_t new_len = P_len + degDiv;
                for (size_t i = 0; i < new_len; ++i) P[i] = TMP[i];
                P_len = new_len;
            }

            /* Dividend D(x) = x^d - 1 */
            seq_t D[1805] = {0};
            D[0] = -1;
            D[d] =  1;
            size_t D_len = d + 1;      /* not really used further */

            /* Division: Φ_d = D / P  (P is monic) */
            size_t degP = P_len - 1;
            size_t Q_len = d - degP + 1;
            seq_t Qtmp[1805] = {0};

            for (int i = (int)d; i >= (int)degP; --i) {
                seq_t qcoeff = D[i];
                size_t qidx = i - degP;
                Qtmp[qidx] = qcoeff;
                if (qcoeff != 0) {
                    for (size_t j = 0; j < P_len; ++j) {
                        D[i - degP + j] -= qcoeff * P[j];
                    }
                }
            }

            /* Store Φ_d */
            phi_deg[idx] = (int)(Q_len - 1);
            for (size_t i = 0; i < Q_len; ++i) phi_coeff[idx][i] = Qtmp[i];
            for (size_t i = Q_len; i <= d; ++i) phi_coeff[idx][i] = 0;
        }

        /* ------------------------------------------------------------------ */
        /* Compute Q(x) = (x^1804-1)/Φ_1804(x) and store it in Qglobal */
        const idx_t m = 1804;
        const size_t idx_phi = num_divs - 1;          /* last divisor is 1804 */
        seq_t D2[1805] = {0};
        D2[0] = -1;
        D2[m] = 1;
        size_t D2_len = m + 1;

        const size_t degPhi = phi_deg[idx_phi];
        const size_t Qlen   = m - degPhi + 1;   /* = 1005 */
        seq_t Qtmp[1805] = {0};

        for (int i = (int)m; i >= (int)degPhi; --i) {
            seq_t qcoeff = D2[i];
            size_t qidx = i - degPhi;
            Qtmp[qidx] = qcoeff;
            if (qcoeff != 0) {
                for (size_t j = 0; j <= degPhi; ++j) {
                    D2[i - degPhi + j] -= qcoeff * phi_coeff[idx_phi][j];
                }
            }
        }
        /* Store Qglobal, zero remaining entries */
        for (size_t i = 0; i < Qlen; ++i) Qglobal[i] = Qtmp[i];
        for (size_t i = Qlen; i <= m; ++i) Qglobal[i] = 0;

        ready = true;
    }

    idx_t r = n % 1804u;
    return -Qglobal[r];
}
