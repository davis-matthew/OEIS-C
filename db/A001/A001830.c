/*
 * A001830  Related to graded partially ordered sets.
 * https://oeis.org/A001830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p+q+r+s+t+u+v=n} (n!/(p!q!r!s!t!u!v!))
 *            2^(pq+qr+rs+st+tu+uv), computed by a DP over the parts: g_k(m,j)
 *            = C(m,j) * Sum_i g_{k-1}(m-j,i) 2^(i*j), g_1(m,j) = [j==m], a(n)
 *            = Sum_j g_7(n,j)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 111 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static void a001830_clear(seq_t *v, size_t len) {
    size_t i;
    for (i = 0; i < len; i++) v[i] = 0;
}

static size_t a001830_ix(idx_t m, idx_t j) {
    return (size_t)(m * (m + 1) / 2 + j);
}

seq_t a(idx_t n) {
    size_t T = (size_t)((n + 1) * (n + 2) / 2);
    seq_t *C = mem_new(seq_t, T);
    seq_t *g = mem_new(seq_t, T);
    seq_t *h = mem_new(seq_t, T);
    seq_t *P = mem_new(seq_t, (size_t)(n + 1));
    idx_t m, j, k;

    P[0] = 1;
    for (j = 1; j <= n; j++) P[j] = P[j - 1] * 2;

    for (m = 0; m <= n; m++) {
        C[a001830_ix(m, 0)] = 1;
        C[a001830_ix(m, m)] = 1;
        for (j = 1; j < m; j++)
            C[a001830_ix(m, j)] = C[a001830_ix(m - 1, j - 1)] + C[a001830_ix(m - 1, j)];
    }

    for (m = 0; m <= n; m++)
        for (j = 0; j <= m; j++)
            g[a001830_ix(m, j)] = (j == m) ? (seq_t)1 : (seq_t)0;

    for (k = 2; k <= 7; k++) {
        for (m = 0; m <= n; m++) {
            for (j = 0; j <= m; j++) {
                idx_t r = m - j;
                seq_t acc = 0;
                idx_t ii;
                for (ii = r + 1; ii-- > 0;)
                    acc = acc * P[j] + g[a001830_ix(r, ii)];
                h[a001830_ix(m, j)] = C[a001830_ix(m, j)] * acc;
            }
        }
        {
            seq_t *tmp = g;
            g = h;
            h = tmp;
        }
    }

    seq_t sum = 0;
    for (j = 0; j <= n; j++) sum += g[a001830_ix(n, j)];

    a001830_clear(C, T);
    a001830_clear(g, T);
    a001830_clear(h, T);
    a001830_clear(P, (size_t)(n + 1));
    mem_free(C);
    mem_free(g);
    mem_free(h);
    mem_free(P);
    return sum;
}
