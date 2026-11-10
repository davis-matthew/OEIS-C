/*
 * A074115  a(n) = n!/A000793(n).
 * https://oeis.org/A074115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!/A000793(n), computed via prime exponents (Legendre) of
 *            n! minus those of Landau's g(n)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 430 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *a074115_g = NULL;
static idx_t a074115_cap = 0;

static void a074115_build(idx_t N) {
    seq_t *g = mem_new(seq_t, (size_t)(N + 1));
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
    if (a074115_g) mem_free(a074115_g);
    a074115_g = g;
    a074115_cap = N;
}

static seq_t A000793_a(idx_t n) {
    if (n == 0) return 1;
    if (a074115_g == NULL || n > a074115_cap) {
        idx_t N = a074115_cap * 2;
        if (N < 1024) N = 1024;
        if (N < n) N = n;
        a074115_build(N);
    }
    return a074115_g[n];
}

seq_t a(idx_t n) {
    seq_t g = A000793_a(n);
    seq_t res = 1;
    idx_t p, q;
    if (n < 2) return 1;
    for (p = 2; p <= n; p++) {
        bool prime = true;
        for (q = 2; q * q <= p; q++) if (p % q == 0) { prime = false; break; }
        if (!prime) continue;
        long long e = 0, eg = 0;
        idx_t t = n;
        while (t > 0) { t /= p; e += (long long)t; }
        while (g > 1 && g % (seq_t)p == 0) { g /= (seq_t)p; eg++; }
        long long d = e - eg;
        while (d > 0) { res *= (seq_t)p; d--; }
    }
    return res;
}
