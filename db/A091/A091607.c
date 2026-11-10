/*
 * A091607  Column 4 of triangle A091602.
 * https://oeis.org/A091607
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A035959(n) - A001935(n), where A035959 = partitions of n
 *            with no part divisible by 5 (g.f. Product_{5∤k} 1/(1-x^k)) and
 *            A001935 = partitions of n with no part divisible by 4 (g.f.
 *            Product_{4∤k} 1/(1-x^k)); both computed by coin-change DP
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 154, u64 n <= 499,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 854 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t diff_dp(idx_t n, idx_t mod, seq_t *out_unused)
{
    (void)out_unused;
    idx_t m = n + 1;
    seq_t *dp = mem_new(seq_t, (size_t)m);
    dp[0] = 1;
    for (idx_t k = 1; k < m; k++) {
        if (k % mod == 0) continue;
        for (idx_t i = k; i < m; i++) {
            dp[i] += dp[i - k];
        }
    }
    seq_t r = dp[n];
    mem_free(dp);
    return r;
}

seq_t a(idx_t n)
{
    static seq_t *c5 = NULL, *c4 = NULL;
    static idx_t len = 0;

    if (n >= len) {
        idx_t m = n + 1;
        if (m < 2 * len) m = 2 * len;
        if (m < 64) m = 64;
        seq_t *d5 = mem_new(seq_t, (size_t)m);
        seq_t *d4 = mem_new(seq_t, (size_t)m);
        d5[0] = 1;
        d4[0] = 1;
        for (idx_t k = 1; k < m; k++) {
            if (k % 5 != 0) {
                for (idx_t i = k; i < m; i++) d5[i] += d5[i - k];
            }
            if (k % 4 != 0) {
                for (idx_t i = k; i < m; i++) d4[i] += d4[i - k];
            }
        }
        if (c5) mem_free(c5);
        if (c4) mem_free(c4);
        c5 = d5;
        c4 = d4;
        len = m;
    }
    return c5[n] - c4[n];
}
