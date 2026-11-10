/*
 * A219922  Smallest m such that n is contained in m-th row of A026835.
 * https://oeis.org/A219922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m such that T(m,k) = n for some k, where T(m,k) =
 *            A026835(m,k) = number of partitions of m into distinct parts all
 *            >= k; computed by search over rows m=1,2,... with a capped
 *            subset-sum DP
 * Range:     u8 n <= 180, u16 n <= 180, u32 n <= 180, u64 n <= 180,
 *            u128 n <= 180, big any n
 * Validated: standard, pass, 82 terms through n = 171 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    uint64_t target = (uint64_t)n;
    uint64_t cap = target + 1;
    for (uint64_t m = 1;; ++m) {
        uint64_t *dp = (uint64_t *)calloc((size_t)m + 1, sizeof(uint64_t));
        if (!dp) return 0;
        dp[0] = 1;
        bool found = false;
        for (uint64_t part = m; part >= 1; --part) {
            for (uint64_t s = m; s >= part; --s) {
                uint64_t v = dp[s] + dp[s - part];
                if (v > cap) v = cap;
                dp[s] = v;
            }
            /* dp[m] now equals T(m, part) (capped) */
            if (dp[m] == target) { found = true; break; }
            if (dp[m] > target) break; /* nonincreasing in k, so no further hits */
        }
        free(dp);
        if (found) return (seq_t)m;
    }
}
