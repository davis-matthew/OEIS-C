/*
 * A003241  Number of achiral rooted trees.
 * https://oeis.org/A003241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Q(1) = 1, Q(s) = N(s-1) with N = Euler transform of Q (rooted
 *            trees, A000081); G(b) = sum over divisors s of b with b/s >= 3
 *            of Q(s); a(n) = Q(n) + sum_{b1<b2, b1+b2=n-1} (N(b1)N(b2) -
 *            G(b1)G(b2)) + [n-1 even] (C(N(h),2) - C(G(h),2)), h = (n-1)/2 -
 *            sum_{s | n-1} Q(s)*min(2, floor(((n-1)/s - 1)/2))
 * Range:     u8 n <= 12, u16 n <= 34, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   A3241_c2, A_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A_grow.h"
#include "../utils/A3241_c2.h"

static seq_t *A3241_Q = NULL, *A3241_N = NULL, *A3241_G = NULL;
static idx_t A3241_cap = 0;

static void A3241_build(idx_t M) {
    idx_t s, b, m;
    idx_t oldlen = A3241_Q ? A3241_cap + 1 : 0;
    A3241_Q = A_grow(A3241_Q, oldlen, M + 1);
    A3241_N = A_grow(A3241_N, oldlen, M + 1);
    A3241_G = A_grow(A3241_G, oldlen, M + 1);
    for (b = 0; b <= M; b++) {
        A3241_Q[b] = 0; A3241_N[b] = 0; A3241_G[b] = 0;
    }
    A3241_Q[1] = 1;
    for (s = 1; s <= M; s++) {
        if (s >= 2) A3241_Q[s] = A3241_N[s - 1];
        for (b = s, m = 1; b <= M; b += s, m++) {
            A3241_N[b] += A3241_Q[s];
            if (m >= 3) A3241_G[b] += A3241_Q[s];
        }
    }
    A3241_cap = M;
}

seq_t a(idx_t n) {
    idx_t N, b1, s;
    seq_t res;
    if (n <= 1) return 1;
    if (n > A3241_cap) {
        idx_t M = 2 * A3241_cap;
        if (M < n) M = n;
        if (M < 64) M = 64;
        A3241_build(M);
    }
    N = n - 1;
    res = A3241_Q[n];
    for (b1 = 1; 2 * b1 < N; b1++) {
        idx_t b2 = N - b1;
        res += A3241_N[b1] * A3241_N[b2] - A3241_G[b1] * A3241_G[b2];
    }
    if (N % 2 == 0) {
        idx_t h = N / 2;
        res += A3241_c2(A3241_N[h]) - A3241_c2(A3241_G[h]);
    }
    for (s = 1; s <= N; s++) {
        if (N % s == 0) {
            idx_t K = N / s;
            idx_t t = (K - 1) / 2;
            if (t > 2) t = 2;
            if (t > 0) res -= A3241_Q[s] * (seq_t)t;
        }
    }
    return res;
}
