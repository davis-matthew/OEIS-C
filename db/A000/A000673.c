/*
 * A000673  Number of bicentered 3-valent (or boron, or binary) trees with n
 *          nodes.
 * https://oeis.org/A000673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{h>=0} [x^n] (E_h(x)^2 + E_h(x^2))/2, where E_h = F_h
 *            - F_{h-1}, F_{-1} = 0, F_h(x) = x*(1 + F_{h-1}(x) +
 *            (F_{h-1}(x)^2 + F_{h-1}(x^2))/2) (F_h = g.f. of rooted trees, at
 *            most 2 children per node, height <= h); a bicentered tree is an
 *            unordered pair of such rooted trees of equal height...
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   a673_sym2, a673_tri2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a673_sym2.h"
#include "../utils/a673_tri2.h"

static seq_t *a673_res = NULL;
static idx_t a673_have = 0;

static void a673_build(idx_t N) {
    seq_t *G = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *F = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *E = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *R = (seq_t *)calloc(N + 1, sizeof(seq_t));
    idx_t k, m;
    for (k = 0; k <= N; k++) { G[k] = 0; F[k] = 0; E[k] = 0; R[k] = 0; }
    for (idx_t h = 0; 2 * (h + 1) <= N; h++) {
        idx_t lim = N - (h + 1);
        F[0] = 0;
        for (k = 1; k <= h && k <= lim; k++) F[k] = G[k];
        for (k = h + 1; k <= lim; k++) {
            idx_t d = k - 1;
            seq_t v = G[d];
            if (d == 0) v += 1;
            v += a673_sym2(G, d, 1);
            F[k] = v;
        }
        for (k = 0; k <= lim; k++) E[k] = F[k] - G[k];
        for (m = 2 * (h + 1); m <= N; m++) R[m] += a673_sym2(E, m, h + 1);
        seq_t *t = G; G = F; F = t;
    }
    free(G); free(F); free(E);
    if (a673_res) free(a673_res);
    a673_res = R;
    a673_have = N + 1;
}

seq_t a(idx_t n) {
    if (n >= a673_have) {
        idx_t N = n + n / 4;
        if (N < 64) N = 64;
        a673_build(N);
    }
    return a673_res[n];
}
