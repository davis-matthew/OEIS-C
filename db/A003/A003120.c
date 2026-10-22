/*
 * A003120  Number of rooted trees with n nodes and omega-valency 1.
 * https://oeis.org/A003120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = [x^(n-1)] (F_0 + F_1 + F_2), where F_s(x) is the g.f. of
 *            forests (multisets of rooted trees) whose omega-valencies sum to
 *            s. A tree of size d has omega 1 if its root's children sum to <=
 *            2, and omega k >= 2 if they sum to k+1. The bivariate forest
 *            table is built by multiplying in (1 - x^d...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   A3120_cap, A3120_sgcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A3120_cap.h"
#include "../utils/A3120_sgcd.h"

static idx_t A3120_M = 0;
static seq_t *A3120_val = NULL;

static void A3120_build(idx_t M) {
    idx_t W = M + 2;
    idx_t rows = M;
    idx_t i, d, k, m, s, j;
    seq_t *P = (seq_t *)calloc((size_t)(rows * W), sizeof(seq_t));
    seq_t *T = (seq_t *)calloc((size_t)W, sizeof(seq_t));
    seq_t *mc = (seq_t *)calloc((size_t)(M + 1), sizeof(seq_t));
    seq_t *val = (seq_t *)calloc((size_t)(M + 1), sizeof(seq_t));
    for (i = 0; i < rows * W; i++) P[i] = 0;
    for (i = 0; i < W; i++) T[i] = 0;
    for (i = 0; i <= M; i++) { mc[i] = 0; val[i] = 0; }
    P[0] = 1;
    for (d = 1; d <= M; d++) {
        idx_t r = d - 1;
        idx_t cr = A3120_cap(r, M);
        idx_t kmax;
        seq_t t1 = 0;
        for (s = 0; s <= 2 && s <= cr; s++) t1 += P[r * W + s];
        val[d] = t1;
        if (d >= M) break;
        kmax = M + 1 - d;
        T[1] = t1;
        for (k = 2; k <= kmax; k++)
            T[k] = (k + 1 <= cr) ? P[r * W + k + 1] : (seq_t)0;
        for (k = 1; k <= kmax; k++) {
            seq_t c = T[k];
            idx_t J;
            if (c == 0) continue;
            J = (M - 1) / d;
            mc[0] = 1;
            for (j = 1; j <= J; j++) {
                seq_t num = c + (j - 1);
                idx_t g = A3120_sgcd((idx_t)(num % j), j);
                idx_t dd = j / g;
                num /= g;
                mc[j] = mc[j - 1] / dd * num;
            }
            for (m = M - 1; m >= d; m--) {
                idx_t capm = A3120_cap(m, M);
                for (s = k; s <= capm; s++) {
                    seq_t acc = 0;
                    for (j = 1; j * d <= m && j * k <= s; j++) {
                        idx_t mm = m - j * d, ss = s - j * k;
                        if (ss > mm) continue;
                        acc += mc[j] * P[mm * W + ss];
                    }
                    if (acc != 0) P[m * W + s] += acc;
                }
            }
        }
    }
    free(P);
    free(T);
    free(mc);
    if (A3120_val) free(A3120_val);
    A3120_val = val;
    A3120_M = M;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > A3120_M) {
        idx_t nm = A3120_M + A3120_M / 4 + 8;
        if (nm < n) nm = n;
        A3120_build(nm);
    }
    return A3120_val[n];
}
