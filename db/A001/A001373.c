/*
 * A001373  Number of functional digraphs (digraphs of functions on n nodes
 *          where every node has outdegree 1 and loops of length 1 are
 *          forbidden).
 * https://oeis.org/A001373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   G.f.: (x/T(x)) / Product_{k>=1} (1 - T(x^k)), where T(x) = Sum
 *            t(n) x^n is the g.f. of A000081 (rooted trees), T(x) = x *
 *            Product_{k>=1} (1 - x^k)^(-t(k)); all series operations are
 *            division-free (exact binomials via gcd reduction)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 44, u128 n <= 85,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 486 (2026-11-03)
 * Helpers:   gcd_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_idx.h"
#include "../utils/mem.h"

static seq_t *a001373_cache = NULL;
static idx_t a001373_len = 0;

static void a001373_build(idx_t N) {
    idx_t i, j, k;
    seq_t *t = mem_new(seq_t, (size_t)(N + 2));
    seq_t *F = mem_new(seq_t, (size_t)(N + 1));
    seq_t *B = mem_new(seq_t, (size_t)(N + 1));
    seq_t *U = mem_new(seq_t, (size_t)(N + 1));
    seq_t *P = mem_new(seq_t, (size_t)(N + 1));
    seq_t *H = mem_new(seq_t, (size_t)(N + 1));
    seq_t *res = mem_new(seq_t, (size_t)(N + 1));

    /* rooted trees: t(k+1) = [x^k] Product (1-x^m)^(-t(m)) */
    F[0] = 1;
    for (k = 1; k <= N + 1; k++) {
        t[k] = F[k - 1];
        if (k > N) break;
        idx_t J = N / k;
        B[0] = 1;
        for (j = 1; j <= J; j++) {
            /* B[j] = C(t(k)+j-1, j) = B[j-1] * (t(k)+j-1) / j */
            seq_t top = t[k] + (seq_t)(j - 1);
            idx_t r = (idx_t)(long long)(top % (seq_t)j);
            idx_t g = gcd_idx(j, r);
            B[j] = (B[j - 1] / (seq_t)(j / g)) * (top / (seq_t)g);
        }
        for (i = N; i >= k; i--) {
            seq_t s = 0;
            for (j = 1; j * k <= i; j++) s += B[j] * F[i - j * k];
            F[i] += s;
        }
    }

    /* U(y) = 1/(1 - T(y)) */
    U[0] = 1;
    for (i = 1; i <= N; i++) {
        seq_t s = 0;
        for (j = 1; j <= i; j++) s += t[j] * U[i - j];
        U[i] = s;
    }

    /* P(x) = Product_{k>=1} U(x^k) */
    P[0] = 1;
    for (k = 1; k <= N; k++) {
        for (i = N; i >= k; i--) {
            seq_t s = 0;
            for (j = 1; j * k <= i; j++) s += U[j] * P[i - j * k];
            P[i] += s;
        }
    }

    /* H(x) = x/T(x) = 1 / (T(x)/x) */
    H[0] = 1;
    for (i = 1; i <= N; i++) {
        seq_t s = 0;
        for (j = 1; j <= i; j++) s += t[j + 1] * H[i - j];
        H[i] = (seq_t)0 - s;
    }

    for (i = 0; i <= N; i++) {
        seq_t s = 0;
        for (j = 0; j <= i; j++) s += P[j] * H[i - j];
        res[i] = s;
    }

    mem_free(t); mem_free(F); mem_free(B); mem_free(U); mem_free(P); mem_free(H);
    if (a001373_cache) mem_free(a001373_cache);
    a001373_cache = res;
    a001373_len = N + 1;
}

seq_t a(idx_t n) {
    if (n >= a001373_len) a001373_build(n);
    return a001373_cache[n];
}
