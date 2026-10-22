/*
 * A000393  Number of n-node rooted trees of height 6.
 * https://oeis.org/A000393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A034823(n) - A001385(n), where T_h(n) = number of n-node
 *            rooted trees of height <= h: T_0(1) = 1, T_0(n) = 0 for n > 1;
 *            T_h(1) = 1, T_h(n+1) = (1/n) * Sum_{k=1..n} c_h(k) * T_h(n+1-k),
 *            c_h(k) = Sum_{d|k} d*T_{h-1}(d) (Euler transform; fixed-width
 *            builds use the equivalent multiset product...
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   tr_binrow, tr_inv, tr_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/tr_binrow.h"
#include "../utils/tr_inv.h"
#include "../utils/tr_width.h"

#define TR_MAXB 10001
#define TR_MAXF 300

/* fixed-width storage */
static seq_t tr_F5[TR_MAXF + 2], tr_F6[TR_MAXF + 2];
static seq_t tr_prev[TR_MAXF + 2], tr_cur[TR_MAXF + 2], tr_G[TR_MAXF + 2], tr_bin[TR_MAXF + 2];
static int tr_fixed_done = 0;

static void tr_fixed_build(int w) {
    idx_t N = TR_MAXF, s, t, m, i;
    int h;
    for (i = 0; i <= N + 1; i++) tr_prev[i] = 0;
    tr_prev[1] = 1;
    for (h = 1; h <= 6; h++) {
        for (t = 0; t <= N + 1; t++) tr_G[t] = 0;
        tr_G[0] = 1;
        for (s = 1; s <= N - 1; s++) {
            if (tr_prev[s] == 0) continue;
            tr_binrow(tr_prev[s], (N - 1) / s, tr_bin, w);
            for (t = N - 1; t >= s; t--) {
                seq_t acc = 0;
                for (m = 1; m * s <= t; m++) acc += tr_G[t - m * s] * tr_bin[m];
                tr_G[t] += acc;
            }
        }
        for (i = 0; i <= N + 1; i++) tr_cur[i] = 0;
        for (t = 0; t < N; t++) tr_cur[t + 1] = tr_G[t];
        if (h == 5) for (i = 0; i <= N; i++) tr_F5[i] = tr_cur[i];
        if (h == 6) for (i = 0; i <= N; i++) tr_F6[i] = tr_cur[i];
        for (i = 0; i <= N + 1; i++) tr_prev[i] = tr_cur[i];
    }
    tr_fixed_done = 1;
}

/* arbitrary-precision storage */
static seq_t tr_L[7][TR_MAXB + 1], tr_C[7][TR_MAXB + 1];
static idx_t tr_have = 0;

static void tr_extend(idx_t n) {
    idx_t i, k, d;
    int h;
    for (i = tr_have + 1; i <= n; i++) {
        tr_L[0][i] = (i == 1) ? 1 : 0;
        for (h = 1; h <= 6; h++) {
            if (i == 1) { tr_L[h][1] = 1; continue; }
            k = i - 1;
            {
                seq_t c = 0;
                for (d = 1; d * d <= k; d++) {
                    if (k % d == 0) {
                        idx_t e = k / d;
                        c += (seq_t)d * tr_L[h - 1][d];
                        if (e != d) c += (seq_t)e * tr_L[h - 1][e];
                    }
                }
                tr_C[h][k] = c;
            }
            {
                seq_t s = 0;
                for (k = 1; k <= i - 1; k++) s += tr_C[h][k] * tr_L[h][i - k];
                tr_L[h][i] = s / (seq_t)(i - 1);
            }
        }
    }
    if (n > tr_have) tr_have = n;
}

seq_t a(idx_t n) {
    int w = tr_width();
    if (w > 0) {
        if (n > TR_MAXF) return 0; /* far beyond any fixed width */
        if (!tr_fixed_done) tr_fixed_build(w);
        return tr_F6[n] - tr_F5[n];
    }
    if (n > TR_MAXB) n = TR_MAXB;
    if (n > tr_have) tr_extend(n);
    return tr_L[6][n] - tr_L[5][n];
}
