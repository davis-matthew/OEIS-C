/*
 * A000675  Number of centered 3-valent (or boron, or binary) trees with n
 *          nodes.
 * https://oeis.org/A000675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   count by radius h (no closed form; built from generating
 *            functions by height). B_h(x) = x*(1 + B_{h-1}(x) + (B_{h-1}(x)^2
 *            + B_{h-1}(x^2))/2), B_{-1} = 0, B_0 = x (rooted trees, each node
 *            with at most 2 children, height <= h). E = B_{h-1} - B_{h-2}
 *            (branches of height exactly h-1), Q = B_{h-2}. a(...
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   a675_ch2, a675_ch3, a675_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a675_ch2.h"
#include "../utils/a675_ch3.h"
#include "../utils/a675_zeros.h"

static seq_t *a675_tab = NULL;
static idx_t a675_N = 0;

static void a675_compute(idx_t N) {
    seq_t *res = a675_zeros(N + 1);
    idx_t i, k, h;
    res[0] = 1;
    if (N >= 1) res[1] = 1;
    if (N >= 3) {
        idx_t K = N - 1;
        seq_t *prev = a675_zeros(K + 1);
        seq_t *cur = a675_zeros(K + 1);
        seq_t *nxt = a675_zeros(K + 1);
        seq_t *E = a675_zeros(K + 1);
        seq_t *d1 = a675_zeros(K + 1);
        seq_t *d2 = a675_zeros(K + 1);
        seq_t *d3 = a675_zeros(K + 1);
        cur[1] = 1;
        for (h = 1; 2 * h + 1 <= N; h++) {
            idx_t hi = K - h;
            for (i = 0; i <= K; i++) {
                E[i] = 0; d1[i] = 0; d2[i] = 0; d3[i] = 0;
            }
            for (i = h; i <= hi; i++) E[i] = cur[i] - prev[i];
            /* multisets of 1, 2, 3 branches of height exactly h-1 */
            for (i = h; i <= hi; i++) {
                seq_t c = E[i];
                seq_t C2 = 0, C3 = 0;
                if (c == 0) continue;
                if (2 * i <= K) C2 = a675_ch2(c);
                if (3 * i <= K) C3 = a675_ch3(c);
                for (k = i + 2 * h; k <= K; k++) {
                    seq_t t = d2[k - i] * c;
                    if (k >= 2 * i + h) t += d1[k - 2 * i] * C2;
                    d3[k] += t;
                }
                if (3 * i <= K) d3[3 * i] += C3;
                for (k = i + h; k <= K; k++) d2[k] += d1[k - i] * c;
                if (2 * i <= K) d2[2 * i] += C2;
                d1[i] += c;
            }
            /* centered trees of radius h */
            for (k = 2 * h; k <= K; k++) {
                idx_t q;
                if (d2[k] == 0 && d3[k] == 0) continue;
                res[k + 1] += d2[k] + d3[k];
                if (d2[k] == 0) continue;
                for (q = 1; k + q <= K; q++) res[k + q + 1] += d2[k] * prev[q];
            }
            /* advance: nxt = B_h */
            {
                idx_t lim = K - h - 1;
                seq_t *tmp;
                for (k = 0; k <= h && k <= K; k++) nxt[k] = cur[k];
                for (k = h + 1; k <= lim; k++) {
                    idx_t m = k - 1;
                    seq_t s = cur[m];
                    for (i = 1; 2 * i < m; i++) s += cur[i] * cur[m - i];
                    if (m % 2 == 0) s += a675_ch2(cur[m / 2]);
                    nxt[k] = s;
                }
                tmp = prev; prev = cur; cur = nxt; nxt = tmp;
            }
        }
        free(prev); free(cur); free(nxt); free(E);
        free(d1); free(d2); free(d3);
    }
    if (a675_tab != NULL) free(a675_tab);
    a675_tab = res;
    a675_N = N;
}

seq_t a(idx_t n) {
    if (a675_tab == NULL || n > a675_N) {
        idx_t newN = n;
        if (a675_tab != NULL) {
            idx_t g = a675_N + a675_N / 4;
            if (g > newN) newN = g;
        }
        if (newN < 32) newN = 32;
        a675_compute(newN);
    }
    return a675_tab[n];
}
