/*
 * A006951  Number of conjugacy classes in GL(n,2).
 * https://oeis.org/A006951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Product_{k>=1} (1-x^k)/(1-2*x^k). Computed as P(x) =
 *            Product_{k>=2} (1-x^k)/(1-2*x^k), which has nonnegative
 *            coefficients; s(n) = 2*s(n-1) + P[n] gives P/(1-2x), and a(n) =
 *            s(n) - s(n-1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *A_tab = NULL;
static idx_t A_cap = 0;

static void build(idx_t N) {
    seq_t *P = alloc_zero(N);
    seq_t *T = alloc_zero(N);
    idx_t k, i;
    seq_t s, prev;
    P[0] = 1;
    for (k = 2; k < N; k++) {
        /* divide by (1 - 2 x^k) */
        for (i = k; i < N; i++)
            P[i] += P[i - k] * 2;
        /* multiply by (1 - x^k) */
        for (i = N - 1; i >= k; i--)
            P[i] -= P[i - k];
    }
    s = 0;
    prev = 0;
    for (i = 0; i < N; i++) {
        s = s * 2 + P[i];
        T[i] = s - prev;
        prev = s;
    }
    release(P, N);
    if (A_tab) release(A_tab, A_cap);
    A_tab = T;
    A_cap = N;
}

seq_t a(idx_t n) {
    if (n >= A_cap) {
        idx_t N = n + 1;
        if (N < 2 * A_cap) N = 2 * A_cap;
        if (N < 64) N = 64;
        build(N);
    }
    return A_tab[n];
}
