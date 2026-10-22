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
 * Helpers:   fixed_a, gcd_small, seq_big, tet, tet_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/fixed_a.h"
#include "../utils/gcd_small.h"
#include "../utils/seq_big.h"
#include "../utils/tet.h"
#include "../utils/tet_idx.h"

static seq_t *g_dp = 0, *g_c = 0;
static idx_t g_have = 0, g_cap = 0;

static seq_t big_a(idx_t n) {
    idx_t i, k;
    if (n >= g_cap) {
        idx_t nc = g_cap * 2;
        idx_t d, m;
        seq_t *ndp, *ncc;
        if (nc < n + 1) nc = n + 1;
        ndp = (seq_t *)malloc((size_t)nc * sizeof(seq_t));
        ncc = (seq_t *)malloc((size_t)nc * sizeof(seq_t));
        for (m = 0; m < nc; m++) { ndp[m] = 0; ncc[m] = 0; }
        for (m = 0; m < g_have; m++) ndp[m] = g_dp[m];
        for (d = 1; d < nc; d++) {
            seq_t v = (seq_t)d * tet(d);
            for (m = d; m < nc; m += d) ncc[m] += v;
        }
        if (g_dp) free(g_dp);
        if (g_c) free(g_c);
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
