/*
 * A000022  Number of centered hydrocarbons with n atoms.
 * https://oeis.org/A000022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 0, a(1) = 1, and a(n) = [x^n] Sum_{R>=1} ( x*(Z(S_{<=4};
 *            T_{R-1}) - Z(S_{<=4}; T_{R-2})) - (T_{R-1} - T_{R-2})*T_{R-1} ),
 *            where T_{-1} = 0, T_0 = x, T_h = x*Z(S_{<=3}; T_{h-1}) is the
 *            g.f. of alkyl radicals (rooted, root degree <= 3) of height <=
 *            h, and Z(S_{<=k}; f) is the g.f. of multise...
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 30, u64 n <= 53, u128 n <= 60,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 60 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* C(c+i-1, i), number of multisets of size i from c types, i <= 4,
   computed without dividing an overflowed product */
static seq_t multichoose(seq_t c, int i)
{
    seq_t v[4], r;
    int t, k4 = -1;
    for (t = 0; t < i; t++) v[t] = c + (seq_t)t;
    if (i >= 3) {
        for (t = 0; t < i; t++)
            if (v[t] % 3 == 0) { v[t] /= 3; break; }
    }
    if (i == 4) {
        for (t = 0; t < 4; t++)
            if (v[t] % 4 == 0) { v[t] /= 4; k4 = t; break; }
        for (t = 0; t < 4; t++)
            if (t != k4 && v[t] % 2 == 0) { v[t] /= 2; break; }
    } else if (i >= 2) {
        for (t = 0; t < i; t++)
            if (v[t] % 2 == 0) { v[t] /= 2; break; }
    }
    r = 1;
    for (t = 0; t < i; t++) r *= v[t];
    return r;
}

/* add c objects of weight d to multiset table M[j][m] (size j, weight m) */
static void add_class(seq_t **M, idx_t d, seq_t c, idx_t deg)
{
    seq_t b[5];
    int i, j;
    idx_t m;
    for (i = 1; i <= 4; i++) b[i] = multichoose(c, i);
    for (j = 4; j >= 1; j--)
        for (m = deg; m >= d; m--) {
            seq_t s = 0;
            for (i = 1; i <= j && (idx_t)i * d <= m; i++)
                s += b[i] * M[j - i][m - (idx_t)i * d];
            M[j][m] += s;
        }
}

static seq_t *alloc_series(size_t sz)
{
    seq_t *p = mem_new(seq_t, sz);
    size_t i;
    for (i = 0; i < sz; i++) p[i] = 0;
    return p;
}

static seq_t *a_cache = NULL;
static idx_t a_cap = 0;   /* a_cache holds a(0..a_cap-1) */

static void compute(idx_t N)
{
    size_t sz = (size_t)N + 1, t;
    idx_t deg = N - 1, R, d, k, n, m;
    seq_t *res = alloc_series(sz);
    seq_t *P = alloc_series(sz), *Q = alloc_series(sz), *Pn = alloc_series(sz);
    seq_t *S4Q = alloc_series(sz);
    seq_t *M[5], *PM[5];
    int j;

    for (j = 0; j < 5; j++) { M[j] = alloc_series(sz); PM[j] = alloc_series(sz); }
    PM[0][0] = 1;
    res[1] = 1;
    P[1] = 1;      /* T_0 = x */
    S4Q[0] = 1;    /* Z(S_{<=4}; 0) = 1 */

    for (R = 1; 2 * R + 1 <= N; R++) {
        /* classes d <= R already have their final counts: share them */
        if (R <= deg && P[R] != 0) add_class(PM, R, P[R], deg);
        for (j = 0; j < 5; j++)
            for (t = 0; t < sz; t++) M[j][t] = PM[j][t];
        for (d = R + 1; d <= deg; d++)
            if (P[d] != 0) add_class(M, d, P[d], deg);

        Pn[0] = 0;
        for (m = 0; m <= deg; m++) {
            seq_t s3 = M[0][m] + M[1][m] + M[2][m] + M[3][m];
            seq_t s4 = s3 + M[4][m];
            res[m + 1] += s4 - S4Q[m];
            S4Q[m] = s4;
            Pn[m + 1] = s3;
        }
        for (k = R; k < N; k++) {
            seq_t bk = P[k] - Q[k];
            if (bk == 0) continue;
            for (n = k + 1; n <= N; n++) res[n] -= bk * P[n - k];
        }
        {
            seq_t *tmp = Q;
            Q = P; P = Pn; Pn = tmp;
        }
    }

    for (j = 0; j < 5; j++) { mem_free(M[j]); mem_free(PM[j]); }
    mem_free(P); mem_free(Q); mem_free(Pn); mem_free(S4Q);
    if (a_cache) mem_free(a_cache);
    a_cache = res;
    a_cap = N + 1;
}

seq_t a(idx_t n)
{
    if (n >= a_cap) compute(n + n / 4 + 16);
    return a_cache[n];
}
