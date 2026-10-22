/*
 * A001833  Number of labeled graded partially ordered sets with n elements.
 * https://oeis.org/A001833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! [x^n] A(x), where A = 1/w and w = e^{-C} (C = e.g.f.
 *            of connected graded posets) is the root near 1 of w*(1 +
 *            sum_{h>=0} B_h(x) w^(h+1)) = 1, B_h = e.g.f. of rank structures
 *            with h+1 nonempty levels and arbitrary edges between consecutive
 *            levels. Computed via V_m = w * x^m/m! * (1 + sum...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Helpers:   norm_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/norm_pair.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void compute(idx_t n)
{
    idx_t N = n + 1, t, m, mp, j, r;
    seq_t *Bn = (seq_t *)calloc(N * N, sizeof(seq_t));
    seq_t *pw = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *E = (seq_t *)calloc(N * N, sizeof(seq_t));
    seq_t *A = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *wp = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *wn = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *vp = (seq_t *)calloc(N * N, sizeof(seq_t));
    seq_t *vn = (seq_t *)calloc(N * N, sizeof(seq_t));
    seq_t *up = (seq_t *)calloc(N * N, sizeof(seq_t));
    seq_t *un = (seq_t *)calloc(N * N, sizeof(seq_t));

    for (t = 0; t < N; t++) {
        Bn[t * N] = 1;
        for (m = 1; m <= t; m++)
            Bn[t * N + m] = (m == t) ? (seq_t)1 : Bn[(t - 1) * N + m - 1] + Bn[(t - 1) * N + m];
    }
    pw[0] = 1;
    for (m = 1; m < N; m++) pw[m] = pw[m - 1] * 2;
    for (m = 1; m < N; m++) {
        E[m * N] = 1;
        for (mp = 1; mp < N; mp++) E[m * N + mp] = E[m * N + mp - 1] * pw[m];
    }

    A[0] = 1;
    wp[0] = 1; wn[0] = 0;
    for (m = 1; m < N; m++) { up[m * N] = 1; un[m * N] = 0; }

    for (t = 1; t <= n; t++) {
        seq_t Ap = 0, An = 0, sp, sn;
        for (m = 1; m <= t; m++) {
            r = t - m;
            sp = 0; sn = 0;
            for (j = 0; j <= r; j++) {
                seq_t c = Bn[r * N + j];
                seq_t a1 = up[m * N + r - j], a2 = un[m * N + r - j];
                sp += c * (wp[j] * a1 + wn[j] * a2);
                sn += c * (wp[j] * a2 + wn[j] * a1);
            }
            norm_pair(&sp, &sn);
            vp[m * N + t] = Bn[t * N + m] * sp;
            vn[m * N + t] = Bn[t * N + m] * sn;
            Ap += vp[m * N + t];
            An += vn[m * N + t];
        }
        A[t] = Ap - An;

        sp = 0; sn = 0;
        for (j = 0; j < t; j++) {
            seq_t c = Bn[t * N + j] * A[t - j];
            sp += c * wn[j];
            sn += c * wp[j];
        }
        norm_pair(&sp, &sn);
        wp[t] = sp; wn[t] = sn;

        for (m = 1; m + t <= n; m++) {
            sp = 0; sn = 0;
            for (mp = 1; mp <= t; mp++) {
                sp += E[m * N + mp] * vp[mp * N + t];
                sn += E[m * N + mp] * vn[mp * N + t];
            }
            norm_pair(&sp, &sn);
            up[m * N + t] = sp; un[m * N + t] = sn;
        }
    }

    free(Bn); free(pw); free(E); free(wp); free(wn);
    free(vp); free(vn); free(up); free(un);
    if (cache) free(cache);
    cache = A;
    cache_len = N;
}

seq_t a(idx_t n)
{
    if (n >= cache_len) compute(n);
    return cache[n];
}
