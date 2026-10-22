/*
 * A002990  Number of n-node trees with a forbidden limb of length 4.
 * https://oeis.org/A002990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   count planted subtrees by type: paths P1..P3 with g.f. x^s, and
 *            branched subtrees B_d, where d is the distance from the root to
 *            the first branching node. T = x + x^2 + x^3 + B, B_0 =
 *            x*(MSET(T) - 1 - T), B_{d+1} = x*B_d (B_{>=3} accumulates).
 *            Count vertex-rooted valid trees V, and subtract unorde...
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 29, u64 n <= 51, u128 n <= 93,
 *            big any n
 * Helpers:   a2990_gcd, a2990_isfixed, a2990_pairs, a2990_tri, a2990_tri0,
 *            a2990_zalloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a2990_gcd.h"
#include "../utils/a2990_isfixed.h"
#include "../utils/a2990_pairs.h"
#include "../utils/a2990_tri.h"
#include "../utils/a2990_tri0.h"
#include "../utils/a2990_zalloc.h"

static seq_t *a2990_res = NULL;
static idx_t a2990_len = 0;

static void a2990_build(idx_t N) {
    int fx = a2990_isfixed();
    idx_t m, i, j, t, k, n;
    idx_t L = N + 1;
    seq_t *T = a2990_zalloc(L), *B0 = a2990_zalloc(L), *B1 = a2990_zalloc(L);
    seq_t *B2 = a2990_zalloc(L), *B3 = a2990_zalloc(L), *Bt = a2990_zalloc(L);
    seq_t *M = a2990_zalloc(L), *c = a2990_zalloc(L), *mc = a2990_zalloc(L + 1);
    seq_t *r = a2990_zalloc(L);
    M[0] = 1;
    for (m = 1; m <= N; m++) {
        if (m >= 2) { B0[m] = M[m - 1]; B0[m] -= T[m - 1]; }
        B1[m] = B0[m - 1];
        B2[m] = B1[m - 1];
        B3[m] = B2[m - 1] + B3[m - 1];
        Bt[m] = B0[m] + B1[m] + B2[m] + B3[m];
        T[m] = Bt[m];
        if (m <= 3) T[m] += 1;
        if (fx) {
            if (T[m] != 0) {
                idx_t J = N / m;
                seq_t res = 1;
                mc[0] = 1;
                for (j = 1; j <= J; j++) {
                    seq_t sj = (seq_t)j, sg, dd, num;
                    idx_t g;
                    if (sj == 0) { for (; j <= J; j++) mc[j] = 0; break; }
                    g = a2990_gcd((idx_t)(res % sj), j);
                    sg = (seq_t)g;
                    dd = (seq_t)(j / g);
                    if (sg == 0 || dd == 0) { for (; j <= J; j++) mc[j] = 0; break; }
                    num = T[m] + (seq_t)(j - 1);
                    res = (res / sg) * (num / dd);
                    mc[j] = res;
                }
                for (t = N; t >= m; t--) {
                    seq_t acc = 0;
                    for (j = 1; j * m <= t; j++) acc += mc[j] * M[t - j * m];
                    M[t] += acc;
                }
            }
        } else {
            seq_t s = 0;
            for (k = m; k <= N; k += m) c[k] += (seq_t)m * T[m];
            for (i = 1; i <= m; i++) s += c[i] * M[m - i];
            M[m] = s / (seq_t)m;
        }
    }
    r[0] = 1;
    for (n = 1; n <= N; n++) {
        seq_t V, E = 0;
        k = n - 1;
        V = M[k];
        V -= T[k];
        V -= a2990_pairs(T, k, 1);
        if (k >= 1 && k <= 3) V += 1;
        V += B0[k] + B1[k] + B2[k];
        if (k == 2 || k == 3) V += 1;
        if (k >= 1) V += B0[k - 1] + B1[k - 1];
        if (k >= 2) V += B0[k - 2];
        V += a2990_pairs(Bt, k, 1);
        if (n == 3 || n == 4) E += 1;
        E += B0[n - 1] + B1[n - 1] + B2[n - 1];
        if (n >= 2) E += B0[n - 2] + B1[n - 2];
        if (n >= 3) E += B0[n - 3];
        E += a2990_pairs(Bt, n, 0);
        V -= E;
        r[n] = V;
    }
    free(T); free(B0); free(B1); free(B2); free(B3); free(Bt);
    free(M); free(c); free(mc);
    if (a2990_res) free(a2990_res);
    a2990_res = r;
    a2990_len = N + 1;
}

seq_t a(idx_t n) {
    if (n >= a2990_len) {
        idx_t N = n;
        if (N < 64) N = 64;
        a2990_build(N);
    }
    return a2990_res[n];
}
