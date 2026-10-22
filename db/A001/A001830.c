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
 * Helpers:   a001830_clear, a001830_ix
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001830_clear.h"
#include "../utils/a001830_ix.h"

seq_t a(idx_t n) {
    size_t T = (size_t)((n + 1) * (n + 2) / 2);
    seq_t *C = (seq_t *)calloc(T, sizeof(seq_t));
    seq_t *g = (seq_t *)calloc(T, sizeof(seq_t));
    seq_t *h = (seq_t *)calloc(T, sizeof(seq_t));
    seq_t *P = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
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
    free(C);
    free(g);
    free(h);
    free(P);
    return sum;
}
