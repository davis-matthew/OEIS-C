/*
 * A080091  Duplicate of A000602.
 * https://oeis.org/A080091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = A000602(n); number of n-node unrooted quartic trees
 *            (alkane isomers). Count trees rooted at centroid(s): with R(s) =
 *            number of rooted trees of s nodes whose root has <= 3 children,
 *            a(n) = multisets of <= 4 R-trees (each of size <= (n-1)/2) of
 *            total size n-1, plus a correction for bicentroi...
 * Range:     i8 n <= 10, i16 n <= 17, i32 n <= 18, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A000602_mc(seq_t b, int j) {
    seq_t f[4];
    int i, used2 = -1;
    seq_t r = 1;
    for (i = 0; i < j; i++) f[i] = b + (seq_t)i;
    if (j >= 3) {
        for (i = 0; i < j; i++) if (f[i] % 3 == 0) { f[i] /= 3; break; }
    }
    if (j == 4) {
        for (i = 0; i < j; i++) if (f[i] % 4 == 0) { f[i] /= 4; used2 = i; break; }
    }
    if (j >= 2) {
        for (i = 0; i < j; i++) if (i != used2 && f[i] % 2 == 0) { f[i] /= 2; break; }
    }
    for (i = 0; i < j; i++) r *= f[i];
    return r;
}

static seq_t *A000602_ans = 0;
static idx_t A000602_N = 0;

static void A000602_build(idx_t N) {
    seq_t *M[5];
    seq_t *ans;
    idx_t t, m, k;
    int j;
    for (k = 0; k < 5; k++) M[k] = mem_new(seq_t, (size_t)(N + 2));
    ans = mem_new(seq_t, (size_t)(N + 2));
    M[0][0] = 1;
    ans[0] = 1;
    if (N >= 1) ans[1] = 1;
    if (N >= 2) ans[2] = 1;
    for (t = 1; 2 * t + 1 <= N; t++) {
        seq_t Rt = M[0][t - 1] + M[1][t - 1] + M[2][t - 1] + M[3][t - 1];
        seq_t c[5];
        for (j = 1; j <= 4; j++) c[j] = A000602_mc(Rt, j);
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
            ans[2 * t + 2] = s + A000602_mc(R2, 2);
        }
    }
    for (k = 0; k < 5; k++) mem_free(M[k]);
    if (A000602_ans) mem_free(A000602_ans);
    A000602_ans = ans;
    A000602_N = N;
}

static seq_t A000602_a(idx_t n) {
    if (A000602_ans == 0 || n > A000602_N) {
        idx_t N = 2 * A000602_N;
        if (N < n) N = n;
        if (N < 64) N = 64;
        A000602_build(N);
    }
    return A000602_ans[n];
}

seq_t a(idx_t n) {
    return A000602_a(n);
}
