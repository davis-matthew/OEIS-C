/*
 * A000793  Landau's function g(n): largest order of permutation of n
 *          elements. Equivalently, largest LCM of partitions of n.
 * https://oeis.org/A000793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, nice
 * Formula:   a(n) = max over partitions of n into prime powers of their
 *            product (Landau's function), computed by a 0/1 knapsack over
 *            primes: for each prime p, g[s] = max(g[s], g[s - p^k] * p^k),
 *            with g[s] = 1 initially; a(n) = g[n]
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 119, u64 n <= 357,
 *            u128 n <= 1109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *a793_g = NULL;
static idx_t a793_cap = 0;

static void a793_build(idx_t N) {
    seq_t *g = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    bool *comp = (bool *)calloc((size_t)(N + 1), sizeof(bool));
    idx_t s, p, i;
    for (s = 0; s <= N; s++) g[s] = 1;
    for (p = 2; p <= N; p++) {
        if (comp[p]) continue;
        for (i = p * p; i <= N; i += p) comp[i] = true;
        for (s = N; s >= p; s--) {
            seq_t best = g[s];
            idx_t pk = p;
            while (pk <= s) {
                seq_t cand = g[s - pk] * (seq_t)pk;
                if (cand > best) best = cand;
                if (pk > N / p) break;
                pk *= p;
            }
            g[s] = best;
        }
    }
    free(comp);
    if (a793_g) free(a793_g);
    a793_g = g;
    a793_cap = N;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (a793_g == NULL || n > a793_cap) {
        idx_t N = a793_cap * 2;
        if (N < 1024) N = 1024;
        if (N < n) N = n;
        a793_build(N);
    }
    return a793_g[n];
}
