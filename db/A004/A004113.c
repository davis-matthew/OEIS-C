/*
 * A004113  Number of rooted trees with n nodes and 2-colored non-leaf nodes.
 * https://oeis.org/A004113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(1) = 1, a(n+1) = 2*b(n) where b = EULER(a), i.e. Sum_{n>=0}
 *            b(n) x^n = Product_{k>=1} 1/(1-x^k)^a(k); computed via b(n) =
 *            (1/n) Sum_{k=1..n} c(k) b(n-k), c(k) = Sum_{d|k} d*a(d), b(0) =
 *            1 (arbitrary precision), or via the division-free multiset
 *            product DP (fixed width)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 33, u128 n <= 63,
 *            big any n
 * Helpers:   a004113_gcd, alloc_zero, grow_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/a004113_gcd.h"

static seq_t *A4113 = NULL;   /* A4113[1..have] valid */
static seq_t *B4113 = NULL;   /* Euler transform b(0..have-1) (big path) */
static seq_t *C4113 = NULL;   /* c(k) (big path) */
static idx_t cap4113 = 0;
static idx_t have4113 = 0;

static void a004113_fixed(idx_t n) {
    idx_t N = n;
    if (N < 2 * have4113) N = 2 * have4113;
    if (N < 64) N = 64;
    seq_t *na = alloc_zero(N + 1);
    seq_t *dp = alloc_zero(N);
    seq_t *cs = alloc_zero(N);
    dp[0] = 1;
    na[1] = 1;
    for (idx_t j = 1; j < N; j++) {
        if (j > 1) na[j] = (seq_t)2 * dp[j - 1];
        seq_t m = na[j];
        idx_t imax = (N - 1) / j;
        seq_t c = 1;
        for (idx_t i = 1; i <= imax; i++) {
            seq_t top = m + (seq_t)(i - 1);
            long long g = a004113_gcd((long long)(top % (seq_t)i), (long long)i);
            c = (c / (seq_t)(i / (idx_t)g)) * (top / (seq_t)g);
            cs[i] = c;
        }
        for (idx_t s = N - 1; s >= j; s--) {
            seq_t add = 0;
            idx_t i = 1;
            for (idx_t t = j; t <= s; t += j, i++)
                add += cs[i] * dp[s - t];
            dp[s] += add;
        }
    }
    na[N] = (N > 1) ? (seq_t)2 * dp[N - 1] : (seq_t)1;
    release(dp, N);
    release(cs, N);
    if (A4113) release(A4113, cap4113);
    A4113 = na;
    cap4113 = N + 1;
    have4113 = N;
}

static void a004113_big(idx_t n) {
    if (n + 1 > cap4113) {
        idx_t nc = 2 * cap4113;
        if (nc < n + 1) nc = n + 1;
        if (nc < 64) nc = 64;
        A4113 = grow_zero(A4113, cap4113, nc);
        B4113 = grow_zero(B4113, cap4113, nc);
        C4113 = grow_zero(C4113, cap4113, nc);
        cap4113 = nc;
    }
    if (have4113 == 0) {
        A4113[1] = 1;
        B4113[0] = 1;
        have4113 = 1;
    }
    while (have4113 < n) {
        idx_t m = have4113;
        seq_t c = 0;
        for (idx_t d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                idx_t e = m / d;
                c += (seq_t)d * A4113[d];
                if (e != d) c += (seq_t)e * A4113[e];
            }
        }
        C4113[m] = c;
        seq_t s = 0;
        for (idx_t k = 1; k <= m; k++)
            s += C4113[k] * B4113[m - k];
        B4113[m] = s / (seq_t)m;
        A4113[m + 1] = (seq_t)2 * B4113[m];
        have4113 = m + 1;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > have4113) {
        if (seq_is_fixed_width()) a004113_fixed(n);
        else a004113_big(n);
    }
    return A4113[n];
}
