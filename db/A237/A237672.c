/*
 * A237672  Number of (n+1) X (1+1) 0..3 arrays with the maximum plus the
 *          upper median plus the minimum of every 2 X 2 subblock differing
 *          from its horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of (n+1)×2 arrays with entries 0..3 for
 *            which, for every pair of consecutive 2×2 subblocks, the value S
 *            = min + third largest + max differs by exactly 1. This is
 *            computed by dynamic programming over row pairs
 * Range:     u8 none, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   precompute_S
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/precompute_S.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)256;   // 4^4
    static uint8_t S[16][16];
    static bool init = false;
    if (!init) {
        precompute_S(S);
        init = true;
    }

    /* dp[prev][last] : number of ways for the current sequence
       ending with rows prev, last */
    static seq_t dp[16][16], ndp[16][16];
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = (seq_t)1;   // all pairs for the first two rows

    /* We need n-1 transitions to reach n subblocks */
    for (idx_t step = 2; step <= n; ++step) {
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev < 16; ++prev) {
            for (int last = 0; last < 16; ++last) {
                seq_t ways = dp[prev][last];
                if (!ways) continue;
                uint8_t s_prev_last = S[prev][last];
                for (int next = 0; next < 16; ++next) {
                    uint8_t s_last_next = S[last][next];
                    int diff = s_prev_last > s_last_next ? s_prev_last - s_last_next
                                                          : s_last_next - s_prev_last;
                    if (diff == 1) {
                        ndp[last][next] += ways;
                    }
                }
            }
        }

        /* swap dp and ndp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* sum all possibilities for the final row pair */
    seq_t result = 0;
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            result += dp[i][j];
    return result;
}
