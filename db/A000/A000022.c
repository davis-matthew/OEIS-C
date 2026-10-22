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
 * Helpers:   add_class, alloc_series, multichoose
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/add_class.h"
#include "../utils/alloc_series.h"
#include "../utils/multichoose.h"

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

    for (j = 0; j < 5; j++) { free(M[j]); free(PM[j]); }
    free(P); free(Q); free(Pn); free(S4Q);
    if (a_cache) free(a_cache);
    a_cache = res;
    a_cap = N + 1;
}

seq_t a(idx_t n)
{
    if (n >= a_cap) compute(n + n / 4 + 16);
    return a_cache[n];
}
