/*
 * A225651  Numbers k that divide A000793(k).
 * https://oeis.org/A225651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that k divides A000793(k) (Landau's
 *            function); computed by a log-domain knapsack with traceback,
 *            taking A000793(k) mod k
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9449 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

static uint64_t *a225651_terms = NULL;
static size_t a225651_cnt = 0;
static uint64_t a225651_N = 0;

static void a225651_build(uint64_t N) {
    double lg = log((double)N);
    uint64_t cap = (uint64_t)(3.0 * sqrt((double)N * lg)) + 60;
    if (cap > N) cap = N;
    size_t i, m = 0;
    uint64_t s, p, q;
    bool *comp = (bool *)calloc((size_t)(cap + 1), sizeof(bool));
    uint64_t *pr = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(cap + 1));
    for (p = 2; p <= cap; p++) {
        if (comp[p]) continue;
        pr[m++] = p;
        for (q = p * p; q <= cap; q += p) comp[q] = true;
    }
    free(comp);
    size_t W = (size_t)(N + 1);
    double *L = (double *)calloc(W, sizeof(double));
    unsigned char *pick = (unsigned char *)calloc(m * W + 1, 1);
    for (i = 0; i < m; i++) {
        p = pr[i];
        for (s = N; s >= p; s--) {
            double best = L[s];
            unsigned char be = 0;
            uint64_t pk = p;
            unsigned char e = 1;
            while (pk <= s) {
                double cand = L[s - pk] + log((double)pk);
                if (cand > best) { best = cand; be = e; }
                if (pk > N / p) break;
                pk *= p;
                e++;
            }
            L[s] = best;
            pick[i * W + (size_t)s] = be;
        }
    }
    free(L);
    free(a225651_terms);
    a225651_terms = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(N + 1));
    a225651_cnt = 0;
    uint64_t k;
    for (k = 1; k <= N; k++) {
        uint64_t mult = 1 % k;
        s = k;
        size_t j = m;
        while (j > 0) {
            j--;
            unsigned char e = pick[j * W + (size_t)s];
            if (e) {
                uint64_t pk = 1;
                unsigned char t;
                for (t = 0; t < e; t++) pk *= pr[j];
                mult = (mult * (pk % k)) % k;
                s -= pk;
            }
        }
        if (mult == 0) a225651_terms[a225651_cnt++] = k;
    }
    free(pick);
    free(pr);
    a225651_N = N;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while (a225651_terms == NULL || (uint64_t)n > a225651_cnt) {
        uint64_t N = a225651_N * 2;
        if (N < 1024) N = 1024;
        a225651_build(N);
    }
    return (seq_t)a225651_terms[(size_t)(n - 1)];
}
