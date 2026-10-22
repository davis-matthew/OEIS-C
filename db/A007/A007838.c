/*
 * A007838  Number of permutations of n elements with distinct cycle lengths.
 * https://oeis.org/A007838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! [x^n] Product_{m>=1} (1 + x^m/m), computed by an
 *            integer knapsack over distinct cycle lengths m: D(s+m) += D(s) *
 *            (s+m)!/(s!*m), starting from D(0) = 1; after all m <= N have
 *            been processed, a(s) = D(s)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *tab = NULL;
static idx_t tabN = 0;
static int have = 0;

static void build(idx_t N) {
    seq_t *D = alloc_zero(N + 1);
    seq_t *w = alloc_zero(N + 1);
    seq_t f = 1; /* (m-1)! */
    D[0] = 1;
    for (idx_t m = 1; m <= N; m++) {
        /* w[s] = (s+m)! / (s! * m) = (m-1)! * binom(s+m, m) */
        w[0] = f;
        for (idx_t s = 0; s + m < N; s++)
            w[s + 1] = w[s] * (seq_t)(s + 1 + m) / (seq_t)(s + 1);
        idx_t s = N - m + 1;
        while (s-- > 0) {
            if (D[s] != 0)
                D[s + m] += D[s] * w[s];
        }
        f *= (seq_t)m;
    }
    release(w, N + 1);
    if (have) release(tab, tabN + 1);
    tab = D;
    tabN = N;
    have = 1;
}

seq_t a(idx_t n) {
    if (!have || n > tabN) {
        idx_t N = have ? tabN + tabN / 2 : 32;
        if (N < n) N = n;
        build(N);
    }
    return tab[n];
}
