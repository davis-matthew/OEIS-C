/*
 * A091606  Column 3 of triangle A091602.
 * https://oeis.org/A091606
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001935(n) - A000726(n), where A001935 counts partitions
 *            of n with no part divisible by 4 and A000726 counts partitions
 *            of n with no part divisible by 3 (coin-change DP for each)
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 160, u64 n <= 527,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 979 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t A091606_count(idx_t n, idx_t mod) {
    seq_t *dp = mem_new(seq_t, (size_t)(n + 1));
    for (idx_t i = 0; i <= n; i++) dp[i] = 0;
    dp[0] = 1;
    for (idx_t k = 1; k <= n; k++) {
        if (k % mod == 0) continue;
        for (idx_t i = k; i <= n; i++) {
            dp[i] += dp[i - k];
        }
    }
    seq_t r = dp[n];
    mem_free(dp);
    return r;
}

seq_t a(idx_t n) {
    seq_t x = A091606_count(n, 4);
    seq_t y = A091606_count(n, 3);
    return x - y;
}
