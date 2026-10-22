/*
 * A000159  a(n) = A058087(n,n-3).
 * https://oeis.org/A000159
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n,3) of the menage hit polynomials (A058087(n,n-3)):
 *            a(n) = Sum_{j=3..n} (-1)^(j-3) * C(j,3) * r_j * (n-j)!, where
 *            r_j = 2n/(2n-j) * C(2n-j,j) = C(2n-j,j) + C(2n-j-1,j-1)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   c3, seq_is_bounded
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/c3.h"
#include "../utils/seq_is_bounded.h"

seq_t a(idx_t n)
{
    seq_t P = 0, Q = 0;
    idx_t j;

    if (n < 3) return 0;

    if (seq_is_bounded()) {
        /* fixed width: division-free (Pascal), exact modulo 2^w */
        idx_t N2 = 2 * n, m, k;
        seq_t *R = (seq_t *)malloc((size_t)(N2 + 1) * sizeof(seq_t));
        seq_t *r = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        for (k = 0; k <= N2; k++) R[k] = 0;
        for (k = 0; k <= n; k++) r[k] = 0;
        R[0] = 1;
        for (m = 1; m <= N2; m++) {
            idx_t cmax = (m < N2 - m) ? m : N2 - m;
            for (k = cmax; k >= 1; k--) R[k] += R[k - 1];
            /* C(m, j) with j = 2n - m */
            {
                idx_t jj = N2 - m;
                if (jj <= m && jj <= n && jj >= 3) r[jj] += R[jj];
            }
            /* C(m, j-1) with j = 2n - m - 1 */
            if (N2 >= m + 1) {
                idx_t jj = N2 - m - 1;
                if (jj >= 3 && jj <= n && jj - 1 <= m) r[jj] += R[jj - 1];
            }
        }
        /* Horner: term j gets multiplied by (n-j)! */
        for (j = 3; j <= n; j++) {
            seq_t c;
            P *= (seq_t)(n - j + 1);
            Q *= (seq_t)(n - j + 1);
            c = (seq_t)c3(j) * r[j];
            if (((j - 3) & 1) == 0) P += c; else Q += c;
        }
        free(R);
        free(r);
        return P - Q;
    } else {
        /* arbitrary precision: exact multiplicative binomials */
        seq_t Bc = 1; /* C(2n-j, j), starting at j = 0 */
        for (j = 0; j <= n; j++) {
            idx_t m = 2 * n - j;
            if (j >= 3) {
                seq_t rj, c;
                rj = Bc * (seq_t)(2 * n);
                rj /= (seq_t)m;
                P *= (seq_t)(n - j + 1);
                Q *= (seq_t)(n - j + 1);
                c = rj * (seq_t)c3(j);
                if (((j - 3) & 1) == 0) P += c; else Q += c;
            }
            if (j < n) {
                /* C(m-1, j) = C(m, j) * (m-j) / m */
                Bc *= (seq_t)(m - j);
                Bc /= (seq_t)m;
                /* C(m-1, j+1) = C(m-1, j) * (m-1-j) / (j+1) */
                Bc *= (seq_t)(m - j - 1);
                Bc /= (seq_t)(j + 1);
            }
        }
        return P - Q;
    }
}
