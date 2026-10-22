/*
 * A006713  Number of 3-edge-colored connected trivalent graphs with 2n
 *          labeled nodes.
 * https://oeis.org/A006713
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   inclusion-exclusion over edges shared by color classes. T(n) =
 *            number of (possibly disconnected) 3-edge-colored simple
 *            trivalent graphs on 2n labeled nodes = Sum_{t+u+a+b+c=n}
 *            (2n)!/((2t)!(2u)!(2a)!(2b)!(2c)!) * 2^t*(2t-1)!!*(-1)^(a+b+c) *
 *            (2a-1)!!(2b-1)!!(2c-1)!! * (2u+2a-1)!!(2u+2b-1)!!(2u+2c-1...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static idx_t a6713_cap = 0;
static seq_t *a6713_conn = NULL;

static void a6713_build(idx_t N)
{
    idx_t M = 2 * N + 1;
    idx_t i, j, u, r, s, x, c, t, n, k, m;
    seq_t *pas = alloc_zero(M * M);
    seq_t *df = alloc_zero(N + 1);
    seq_t *gp = alloc_zero(N + 1);
    seq_t *gn = alloc_zero(N + 1);
    seq_t *g = alloc_zero(N + 1);
    seq_t *P = alloc_zero(N + 1);
    seq_t *T = alloc_zero(N + 1);
    seq_t *conn = alloc_zero(N + 1);

    for (i = 0; i < M; i++) {
        pas[i * M] = 1;
        for (j = 1; j <= i; j++) {
            pas[i * M + j] = pas[(i - 1) * M + j - 1];
            if (j < i) pas[i * M + j] += pas[(i - 1) * M + j];
        }
    }
    df[0] = 1;
    for (i = 1; i <= N; i++) df[i] = df[i - 1] * (seq_t)(2 * i - 1);

    for (u = 0; u <= N; u++) {
        idx_t R = N - u;
        for (x = 0; x <= R; x++) g[x] = df[x] * df[u + x];
        for (s = 0; s <= R; s++) {
            seq_t acc = 0;
            for (x = 0; x <= s; x++)
                acc += pas[(2 * s) * M + 2 * x] * g[x] * g[s - x];
            P[s] = acc;
        }
        for (r = 0; r <= R; r++) {
            seq_t H = 0, term;
            for (c = 0; c <= r; c++)
                H += pas[(2 * r) * M + 2 * c] * g[c] * P[r - c];
            m = u + r;
            term = pas[(2 * m) * M + 2 * u] * H;
            if (r % 2 == 0) gp[m] += term;
            else gn[m] += term;
        }
    }

    for (n = 0; n <= N; n++) {
        seq_t pos = 0, neg = 0, pw = 1;
        for (t = 0; t <= n; t++) {
            seq_t coef = pas[(2 * n) * M + 2 * t] * df[t] * pw;
            pos += coef * gp[n - t];
            neg += coef * gn[n - t];
            pw *= 2;
        }
        T[n] = pos - neg;
    }

    for (n = 1; n <= N; n++) {
        seq_t sub = 0;
        for (k = 1; k < n; k++)
            sub += pas[(2 * n - 1) * M + 2 * k - 1] * conn[k] * T[n - k];
        conn[n] = T[n] - sub;
    }

    release(pas, M * M);
    release(df, N + 1);
    release(gp, N + 1);
    release(gn, N + 1);
    release(g, N + 1);
    release(P, N + 1);
    release(T, N + 1);

    if (a6713_conn) release(a6713_conn, a6713_cap + 1);
    a6713_conn = conn;
    a6713_cap = N;
}

seq_t a(idx_t n)
{
    if (a6713_conn == NULL || n > a6713_cap) {
        idx_t N = n < 4 ? 4 : n;
        a6713_build(N);
    }
    return a6713_conn[n];
}
