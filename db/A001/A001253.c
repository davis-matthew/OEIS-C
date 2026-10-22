/*
 * A001253  Number of permutations of order n with the length of longest run
 *          equal to 5.
 * https://oeis.org/A001253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = G_5(n) - G_4(n), where G_k(n) counts permutations of [n]
 *            whose maximal monotone runs (adjacent runs share an endpoint)
 *            all have at most k elements. G_k is computed by a DP over
 *            (relative rank of last element, run length) for ascending
 *            endings, using up/down symmetry (complement) for descen...
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

/* out[i] = G(i) for 1 <= i <= N, where runs have at most m steps (m+1 elements) */
static void run_dp(idx_t N, idx_t m, seq_t *out) {
    idx_t W = N + 2;
    seq_t *U = alloc_zero(W * m);
    seq_t *V = alloc_zero(W * m);
    seq_t *T = alloc_zero(W);
    seq_t *P = alloc_zero(W);
    seq_t *acc = alloc_zero(m);
    idx_t i, j, r, l;

    out[1] = 1;
    if (N >= 2) {
        U[2 * m + 0] = 1; /* perm 12: last rank 2, ascending, 1 step */
        out[2] = 2;
    }
    for (i = 2; i < N; i++) {
        /* T[j] = total ascending-ending count at last rank j; P prefix sums */
        P[0] = 0;
        for (j = 1; j <= i; j++) {
            seq_t s = 0;
            for (l = 0; l < m; l++) s += U[j * m + l];
            T[j] = s;
            P[j] = P[j - 1] + s;
        }
        for (l = 0; l < m; l++) acc[l] = 0;
        seq_t total = 0;
        for (r = 1; r <= i + 1; r++) {
            if (r >= 2)
                for (l = 0; l < m; l++) acc[l] += U[(r - 1) * m + l];
            /* coming from a descending run (by symmetry: ranks i+2-r..i) */
            V[r * m + 0] = P[i] - P[i + 1 - r];
            total += V[r * m + 0];
            /* extending an ascending run */
            for (l = 1; l < m; l++) {
                V[r * m + l] = acc[l - 1];
                total += V[r * m + l];
            }
        }
        out[i + 1] = total + total;
        seq_t *tmp = U; U = V; V = tmp;
    }
    release(U, W * m);
    release(V, W * m);
    release(T, W);
    release(P, W);
    release(acc, m);
}

static seq_t *cache = NULL;
static idx_t cached = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > cached) {
        idx_t N = n;
        seq_t *g5 = alloc_zero(N + 1);
        seq_t *g4 = alloc_zero(N + 1);
        seq_t *c = alloc_zero(N + 1);
        idx_t i;
        run_dp(N, 4, g5);
        run_dp(N, 3, g4);
        for (i = 1; i <= N; i++) c[i] = g5[i] - g4[i];
        release(g5, N + 1);
        release(g4, N + 1);
        if (cache) release(cache, cached + 1);
        cache = c;
        cached = N;
    }
    return cache[n];
}
