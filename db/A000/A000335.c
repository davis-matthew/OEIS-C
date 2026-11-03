/*
 * A000335  Euler transform of A000292.
 * https://oeis.org/A000335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   Euler transform of b(k) = A000292(k) = k*(k+1)*(k+2)/6:
 *            Product_{k>=1} 1/(1-x^k)^b(k) = Sum_{n>=0} a(n) x^n, a(0) = 1;
 *            equivalently a(n) = (1/n) * Sum_{k=1..n} c(k)*a(n-k), c(k) =
 *            Sum_{d|k} d*b(d)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 57, u128 n <= 134,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 490 (2026-11-03)
 * Helpers:   gcd_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd_idx.h"
#include "../utils/mem.h"

static idx_t tet_idx(idx_t k) {
    return k * (k + 1) / 2 * (k + 2) / 3;
}

static seq_t fixed_a(idx_t n) {
    idx_t k, m, j;
    seq_t *dp, *C, res;
    if (n > 400) return 0; /* no such term fits in 128 bits */
    dp = mem_new(seq_t, (size_t)(n + 1));
    C = mem_new(seq_t, (size_t)(n + 1));
    for (m = 0; m <= n; m++) dp[m] = 0;
    dp[0] = 1;
    for (k = 1; k <= n; k++) {
        idx_t b = tet_idx(k);
        idx_t J = n / k;
        C[0] = 1;
        /* C[j] = binomial(b+j-1, j) = C[j-1]*(b+j-1)/j */
        for (j = 1; j <= J; j++) {
            idx_t r = (idx_t)(C[j - 1] % (seq_t)j);
            idx_t g = gcd_idx(r, j);
            idx_t q = j / g;
            C[j] = (C[j - 1] / (seq_t)g) * (seq_t)((b + (j - 1)) / q);
        }
        for (m = n; m >= k; m--) {
            seq_t s = dp[m];
            for (j = 1; j * k <= m; j++)
                s += C[j] * dp[m - j * k];
            dp[m] = s;
        }
    }
    res = dp[n];
    mem_free(dp);
    mem_free(C);
    return res;
}

static int seq_big(void) {
    static int r = -1;
    if (r < 0) {
        seq_t t = 1;
        int i;
        for (i = 0; i < 200; i++) t *= 2;
        r = (t != 0) ? 1 : 0;
    }
    return r;
}

static seq_t tet(idx_t k) {
    return (seq_t)tet_idx(k);
}

static seq_t *g_dp = 0, *g_c = 0;
static idx_t g_have = 0, g_cap = 0;

static seq_t big_a(idx_t n) {
    idx_t i, k;
    if (n >= g_cap) {
        idx_t nc = g_cap * 2;
        idx_t d, m;
        seq_t *ndp, *ncc;
        if (nc < n + 1) nc = n + 1;
        ndp = mem_new(seq_t, (size_t)nc);
        ncc = mem_new(seq_t, (size_t)nc);
        for (m = 0; m < nc; m++) { ndp[m] = 0; ncc[m] = 0; }
        for (m = 0; m < g_have; m++) ndp[m] = g_dp[m];
        for (d = 1; d < nc; d++) {
            seq_t v = (seq_t)d * tet(d);
            for (m = d; m < nc; m += d) ncc[m] += v;
        }
        if (g_dp) mem_free(g_dp);
        if (g_c) mem_free(g_c);
        g_dp = ndp;
        g_c = ncc;
        g_cap = nc;
    }
    for (i = g_have; i <= n; i++) {
        if (i == 0) {
            g_dp[0] = 1;
        } else {
            seq_t s = 0;
            for (k = 1; k <= i; k++) s += g_c[k] * g_dp[i - k];
            g_dp[i] = s / (seq_t)i;
        }
    }
    if (n + 1 > g_have) g_have = n + 1;
    return g_dp[n];
}

seq_t a(idx_t n) {
    if (seq_big()) return big_a(n);
    return fixed_a(n);
}
