/*
 * A005435  Number of column-convex polyominoes with perimeter 2n+2.
 * https://oeis.org/A005435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = [x^(n+1)] (1-x)*U(x)/(1-x+U(x)), where U(t^2) is the even
 *            part of g(t), and g = t^4 + (t+t^2+t^3)*g + g^2 (power series in
 *            t). This comes from the kernel method on the column-by-column
 *            (Temperley) functional equation, as in Brak et al. Counting is
 *            by perimeter, with t marking one unit of p...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 28, u128 n <= 54,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *cc = NULL;   /* coefficients of g(t) */
static idx_t ccap = 0, cdone = 0;
static seq_t *gg = NULL;   /* coefficients of G(x) */
static idx_t gcap = 0, gdone = 0;

seq_t a(idx_t n) {
    idx_t M = n + 1;
    idx_t K = 2 * M + 1;
    idx_t k, i, m, j;

    if (K > ccap) {
        idx_t nc = K + K / 2 + 8;
        cc = grow_zero(cc, ccap, nc);
        ccap = nc;
    }
    for (k = cdone; k < K; k++) {
        seq_t s = 0;
        if (k >= 4) {
            if (k == 4) s = 1;
            s += cc[k - 1] + cc[k - 2] + cc[k - 3];
            for (i = 4; i < k - i; i++) {
                seq_t p = cc[i] * cc[k - i];
                s += p + p;
            }
            if (k % 2 == 0 && k / 2 >= 4) s += cc[k / 2] * cc[k / 2];
        }
        cc[k] = s;
    }
    if (K > cdone) cdone = K;

    if (M + 1 > gcap) {
        idx_t nc = M + 1 + (M + 1) / 2 + 8;
        gg = grow_zero(gg, gcap, nc);
        gcap = nc;
    }
    for (m = gdone; m <= M; m++) {
        seq_t pos, neg;
        if (m < 2) { gg[m] = 0; continue; }
        /* u_j = cc[2j];  g_m = g_{m-1} + u_m - u_{m-1} - sum_{j=2..m} u_j g_{m-j} */
        pos = gg[m - 1] + cc[2 * m];
        neg = cc[2 * m - 2];
        for (j = 2; j <= m; j++) neg += cc[2 * j] * gg[m - j];
        gg[m] = pos - neg;
    }
    if (M + 1 > gdone) gdone = M + 1;

    return gg[M];
}
