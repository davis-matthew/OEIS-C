/*
 * A007262  McKay-Thompson series of class 6c for Monster.
 * https://oeis.org/A007262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. A(q) = (E(q)/E(q^3))^6 where E(q) = Product_{k>=1} (1 -
 *            q^k), i.e. a(n) = [q^n] (Product_{k>=1, 3 does not divide k} (1
 *            - q^k))^6 (the g.f. (E(q^2)/E(q^6))^6/q with the zero terms
 *            removed, q^2 -> q)
 * Range:     i8 n <= 6, i16 n <= 21, i32 n <= 72, i64 n <= 267, i128 n <= 999,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *cache = NULL;
static idx_t clen = 0;

/* d += x*y truncated to L terms (d distinct from x, y) */
static void mul_add(seq_t *d, const seq_t *x, const seq_t *y, idx_t L) {
    for (idx_t i = 0; i < L; i++) {
        if (x[i] == 0) continue;
        for (idx_t j = 0; i + j < L; j++) {
            d[i + j] += x[i] * y[j];
        }
    }
}

static void compute(idx_t L) {
    seq_t *g = alloc_zero(L);
    g[0] = 1;
    for (idx_t k = 1; k < L; k++) {
        if (k % 3 == 0) continue;
        for (idx_t i = L - 1; i >= k; i--) {
            g[i] -= g[i - k];
        }
    }
    seq_t *g2 = alloc_zero(L);
    mul_add(g2, g, g, L);
    seq_t *g3 = alloc_zero(L);
    mul_add(g3, g2, g, L);
    release(g2, L);
    release(g, L);
    seq_t *g6 = alloc_zero(L);
    mul_add(g6, g3, g3, L);
    release(g3, L);
    if (cache) release(cache, clen);
    cache = g6;
    clen = L;
}

seq_t a(idx_t n) {
    if (n + 1 > clen) {
        idx_t L = n + 1;
        if (L < 2 * clen) L = 2 * clen;
        if (L < 64) L = 64;
        compute(L);
    }
    return cache[n];
}
