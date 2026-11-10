/*
 * A058611  McKay-Thompson series of class 29A for Monster.
 * https://oeis.org/A058611
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. sum_{n>=-1} a(n) x^n = -2 + x^(-1) * ( G(x)*G(x^29) +
 *            x^6*H(x)*H(x^29) )^2, G = g.f. of A003114, H = g.f. of A003106
 * Range:     u8 n <= 14, u16 n <= 41, u32 n <= 134, u64 n <= 462,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 878 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* coefficients of prod 1/(1-x^k) over parts k with k%5 == r1 or r2, up to x^len-1 */
static seq_t *A058611_parts(idx_t len, int r1, int r2) {
    seq_t *dp = mem_new(seq_t, (size_t)len);
    dp[0] = 1;
    for (idx_t part = 1; part < len; part++) {
        int r = (int)(part % 5);
        if (r != r1 && r != r2) continue;
        for (idx_t i = part; i < len; i++) dp[i] += dp[i - part];
    }
    return dp;
}

seq_t a(idx_t n) {
    idx_t N = n + 1;
    if (N < 0) return 0;
    idx_t len = N + 1;
    seq_t *g = A058611_parts(len, 1, 4);
    seq_t *h = A058611_parts(len, 2, 3);
    seq_t *B = mem_new(seq_t, (size_t)len);
    for (idx_t m = 0; m <= N; m++) {
        seq_t s = 0;
        for (idx_t j = 0; 29 * j <= m; j++) s += g[m - 29 * j] * g[j];
        if (m >= 6) {
            for (idx_t j = 0; 29 * j <= m - 6; j++) s += h[m - 6 - 29 * j] * h[j];
        }
        B[m] = s;
    }
    seq_t r = 0;
    for (idx_t i = 0; i <= N; i++) r += B[i] * B[N - i];
    if (N == 1) r -= 2;
    mem_free(g);
    mem_free(h);
    mem_free(B);
    return r;
}
