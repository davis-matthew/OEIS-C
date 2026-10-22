/*
 * A000602  Number of n-node unrooted quartic trees; number of n-carbon
 *          alkanes C(n)H(2n+2) ignoring stereoisomers.
 * https://oeis.org/A000602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   count trees rooted at their centroid(s). Let R(s) = A000598(s),
 *            the number of rooted trees with s nodes whose root has <= 3
 *            children: R(s) = number of multisets of <= 3 R-trees of total
 *            size s-1. Then a(n) = number of multisets of <= 4 R-trees, each
 *            of size <= floor((n-1)/2), of total size n-1, p...
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 30, u64 n <= 52, u128 n <= 97,
 *            big any n
 * Helpers:   q602_mc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/q602_mc.h"

static seq_t *q602_ans = 0;
static idx_t q602_N = 0;

static void q602_build(idx_t N) {
    seq_t *M[5];
    seq_t *ans;
    idx_t t, m, k;
    int j;
    for (k = 0; k < 5; k++) M[k] = (seq_t *)calloc((size_t)(N + 2), sizeof(seq_t));
    ans = (seq_t *)calloc((size_t)(N + 2), sizeof(seq_t));
    M[0][0] = 1;
    ans[0] = 1;
    if (N >= 1) ans[1] = 1;
    if (N >= 2) ans[2] = 1;
    for (t = 1; 2 * t + 1 <= N; t++) {
        seq_t Rt = M[0][t - 1] + M[1][t - 1] + M[2][t - 1] + M[3][t - 1];
        seq_t c[5];
        for (j = 1; j <= 4; j++) c[j] = q602_mc(Rt, j);
        for (k = 4; k >= 1; k--) {
            idx_t hi = k * t;
            if (hi > N) hi = N;
            for (m = hi; m >= t; m--) {
                seq_t s = 0;
                for (j = 1; (idx_t)j <= k && (idx_t)j * t <= m; j++)
                    s += M[k - j][m - (idx_t)j * t] * c[j];
                M[k][m] += s;
                if (m == 0) break;
            }
        }
        {
            seq_t s = 0;
            for (k = 0; k <= 4; k++) s += M[k][2 * t];
            ans[2 * t + 1] = s;
        }
        if (2 * t + 2 <= N) {
            seq_t s = 0;
            seq_t R2 = M[0][t] + M[1][t] + M[2][t] + M[3][t];
            for (k = 0; k <= 4; k++) s += M[k][2 * t + 1];
            ans[2 * t + 2] = s + q602_mc(R2, 2);
        }
    }
    for (k = 0; k < 5; k++) free(M[k]);
    if (q602_ans) free(q602_ans);
    q602_ans = ans;
    q602_N = N;
}

seq_t a(idx_t n) {
    if (q602_ans == 0 || n > q602_N) {
        idx_t N = 2 * q602_N;
        if (N < n) N = n;
        if (N < 64) N = 64;
        q602_build(N);
    }
    return q602_ans[n];
}
