/*
 * A002809  Increasing values of A000793 (largest order of permutation of n
 *          elements).
 * https://oeis.org/A002809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n-th distinct value of A000793 (Landau's function g(m));
 *            g(m) is computed by a knapsack DP over prime powers p^k with sum
 *            <= m, maximizing the product
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 58, u64 n <= 149,
 *            u128 n <= 395, big any n
 * Helpers:   a001028_grow, a001028_z
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/a001028_grow.h"
#include "../utils/a001028_z.h"

static seq_t *G = NULL;
static double *L = NULL;
static idx_t M = 0;
static uint64_t *pos = NULL;
static idx_t cnt = 0;

static void build(idx_t newM) {
    idx_t i;
    if (G == NULL) G = a001028_z(newM + 1);
    else G = a001028_grow(G, M + 1, newM + 1);
    L = (double *)realloc(L, (size_t)(newM + 1) * sizeof(double));
    for (i = 0; i <= newM; i++) { G[i] = 1; L[i] = 0.0; }
    M = newM;

    double lm = log((double)M + 2.0);
    uint64_t bound = (uint64_t)(2.0 * sqrt((double)M * lm)) + 20;
    if (bound > M) bound = M;

    unsigned char *comp = (unsigned char *)calloc((size_t)bound + 2, 1);
    for (uint64_t p = 2; p <= bound; p++) {
        if (comp[p]) continue;
        for (uint64_t j = p * p; j <= bound; j += p) comp[j] = 1;
        double lp = log((double)p);
        for (uint64_t m = M; m >= p; m--) {
            uint64_t q = p;
            seq_t qs = (seq_t)p;
            int k = 1;
            while (q <= m) {
                double cl = L[m - q] + k * lp;
                if (cl > L[m] + 1e-7) {
                    G[m] = G[m - q] * qs;
                    L[m] = cl;
                } else if (cl > L[m] - 1e-7) {
                    seq_t c = G[m - q] * qs;
                    if (c > G[m]) { G[m] = c; L[m] = cl; }
                }
                q *= p;
                qs *= p;
                k++;
            }
        }
    }
    free(comp);

    pos = (uint64_t *)realloc(pos, (size_t)(M + 1) * sizeof(uint64_t));
    cnt = 0;
    pos[cnt++] = 0;
    for (uint64_t m = 1; m <= M; m++) {
        if (G[m] > G[pos[cnt - 1]]) pos[cnt++] = m;
    }
}

seq_t a(idx_t n) {
    while (cnt < n) {
        build(M ? M * 2 : 64);
    }
    return G[pos[n - 1]];
}
