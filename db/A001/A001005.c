/*
 * A001005  Number of ways of partitioning n points on a circle into subsets
 *          only of sizes 2 and 3.
 * https://oeis.org/A001005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} C(n,k-1)*C(k,n-2k)/k, computed via
 *            the equivalent P-recurrence (arbitrary precision):
 *            2*(n+1)*(2*n+3)*(26*n+1)*a(n) = -(n-1)*(26*n^2+53*n+18)*a(n-1) +
 *            6*(n-1)*(78*n^2+42*n-25)*a(n-2) +
 *            31*(n-2)*(n-1)*(26*n+27)*a(n-3), a(0) = 1, a(1) = 0; and for
 *            fixed-width seq_t via...
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 29, u64 n <= 53, u128 n <= 100,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 200 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t a001005_dp(idx_t n) {
    idx_t M = n + 1, m, i;
    seq_t *t = mem_new(seq_t, (size_t)(M + 1));
    seq_t *s2 = mem_new(seq_t, (size_t)(M + 1));
    seq_t r;
    for (i = 0; i <= M; i++) { t[i] = 0; s2[i] = 0; }
    t[1] = 1;
    for (m = 2; m <= M; m++) {
        idx_t j = m - 1;
        seq_t a2 = 0, a3 = 0;
        for (i = 1; i + 1 <= j; i++) a2 += t[i] * t[j - i];
        s2[j] = a2;
        for (i = 1; i + 2 <= j; i++) a3 += t[i] * s2[j - i];
        t[m] = a2 + a3;
    }
    r = t[M];
    mem_free(t);
    mem_free(s2);
    return r;
}

static int a001005_fixed(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 200; i++) x = x + x;
    return x == 0;
}

static seq_t a001005_rec(idx_t n) {
    seq_t p3 = 0, p2 = 1, p1 = 0; /* a(k-3), a(k-2), a(k-1) */
    idx_t k;
    if (n == 0) return 1;
    if (n == 1) return 0;
    for (k = 2; k <= n; k++) {
        uint64_t c1 = (uint64_t)(k - 1) * (26 * (uint64_t)k * k + 53 * (uint64_t)k + 18);
        uint64_t c2 = 6 * (uint64_t)(k - 1) * (78 * (uint64_t)k * k + 42 * (uint64_t)k - 25);
        uint64_t c3 = 31 * (uint64_t)(k - 2) * (uint64_t)(k - 1) * (26 * (uint64_t)k + 27);
        uint64_t d = 2 * (uint64_t)(k + 1) * (2 * (uint64_t)k + 3) * (26 * (uint64_t)k + 1);
        seq_t pos = (seq_t)c2 * p2 + (seq_t)c3 * p3;
        seq_t neg = (seq_t)c1 * p1;
        seq_t cur = (pos - neg) / (seq_t)d;
        p3 = p2;
        p2 = p1;
        p1 = cur;
    }
    return p1;
}

seq_t a(idx_t n) {
    if (a001005_fixed()) return a001005_dp(n);
    return a001005_rec(n);
}
