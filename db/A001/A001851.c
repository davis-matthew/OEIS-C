/*
 * A001851  Total diameter of unlabeled trees with n nodes.
 * https://oeis.org/A001851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_d d*t(n,d), t(n,d) = number of trees with n nodes and
 *            diameter d. R_h(x) = g.f. of rooted trees of height <= h: R_{-1}
 *            = 0, R_0 = x, R_h = x*EulerT(R_{h-1}); T_h = R_h - R_{h-1}
 *            (height exactly h). Center vertex: t(.,2h) = R_h - R_{h-1} -
 *            T_{h-1}*R_{h-1} (at least two branches of heigh...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   from_w, mc_fixed, smul, zarr_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/from_w.h"
#include "../utils/mc_fixed.h"
#include "../utils/smul.h"
#include "../utils/zarr_s.h"

static int fixed_flag = -1;
static int fixed_s(void)
{
    if (fixed_flag < 0) {
        seq_t x = 1;
        int i;
        for (i = 0; i < 300; i++) x = x + x;
        fixed_flag = (x == 0);
    }
    return fixed_flag;
}

/* E[0..L-1] = Euler transform of R (R[0] = 0), truncated */
static void euler(const seq_t *R, seq_t *E, seq_t *c, seq_t *M, idx_t L)
{
    idx_t m, k, j;
    E[0] = 1;
    for (m = 1; m < L; m++) E[m] = 0;
    if (fixed_s()) {
        for (k = 1; k < L; k++) {
            idx_t J;
            if (R[k] == 0) continue;
            J = (L - 1) / k;
            for (j = 1; j <= J; j++) M[j] = mc_fixed(R[k], j);
            for (m = L - 1; m >= k; m--) {
                seq_t s = 0;
                for (j = 1; j * k <= m; j++) s += smul(M[j], E[m - j * k]);
                E[m] += s;
            }
        }
    } else {
        for (k = 1; k < L; k++) c[k] = 0;
        for (k = 1; k < L; k++)
            if (!(R[k] == 0)) {
                seq_t v = R[k] * k;
                for (m = k; m < L; m += k) c[m] += v;
            }
        for (m = 1; m < L; m++) {
            seq_t s = 0;
            for (k = 1; k <= m; k++) s += c[k] * E[m - k];
            E[m] = s / m;
        }
    }
}

static void compute(idx_t N, seq_t *ans)
{
    idx_t L = N + 1, m, i, h;
    seq_t *A = zarr_s(L), *B = zarr_s(L), *C = zarr_s(L);
    seq_t *E = zarr_s(L), *c = zarr_s(L), *M = zarr_s(L + 1), *T = zarr_s(L);
    seq_t *tmp;
    /* A = R_{h-2}, B = R_{h-1}, C = R_h */
    for (m = 0; m < L; m++) ans[m] = 0;
    B[1] = 1;                 /* R_0 = x */
    if (N >= 2) ans[2] += 1;  /* diameter 1: the single edge */
    for (h = 1; 2 * h <= N - 1; h++) {
        euler(B, E, c, M, N);
        C[0] = 0;
        for (m = 1; m <= N; m++) C[m] = E[m - 1];

        /* even diameter 2h */
        for (i = 0; i < L; i++) T[i] = B[i] - A[i];
        for (m = 1; m <= N; m++) {
            seq_t p = 0, d;
            for (i = h; i < m; i++) p += smul(T[i], B[m - i]);
            d = C[m] - B[m];
            d -= p;
            ans[m] += smul(d, (seq_t)(2 * h));
        }

        /* odd diameter 2h+1 */
        for (i = 0; i < L; i++) T[i] = C[i] - B[i];
        for (m = 2 * h + 2; m <= N; m++) {
            seq_t s = 0;
            for (i = h + 1; 2 * i < m; i++) s += smul(T[i], T[m - i]);
            if (m % 2 == 0) {
                seq_t t = T[m / 2];
                if (t % 2 == 0) s += smul(t / 2, t + 1);
                else s += smul(t, (t + 1) / 2);
            }
            ans[m] += smul(s, (seq_t)(2 * h + 1));
        }

        tmp = A; A = B; B = C; C = tmp;
    }
    free(A); free(B); free(C); free(E); free(c); free(M); free(T);
}

static seq_t *cache = 0;
static idx_t cacheN = 0;

seq_t a(idx_t n)
{
    /* in fixed-width builds a(n) exceeds 128 bits long before n = 200 */
    if (fixed_s() && n > 200) return 0;
    if (n > cacheN) {
        idx_t N = n;
        seq_t *ans;
        if (N < cacheN + cacheN / 2) N = cacheN + cacheN / 2;
        if (N < 8) N = 8;
        if (fixed_s() && N > 200) N = (n > 200) ? n : 200;
        ans = zarr_s(N + 1);
        compute(N, ans);
        if (cache) free(cache);
        cache = ans;
        cacheN = N;
    }
    return cache[n];
}
