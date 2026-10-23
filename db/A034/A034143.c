/*
 * A034143  Number of partitions of n into distinct parts from [1, 13].
 * https://oeis.org/A034143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts from [1,13]
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    const idx_t MAX_SUM = 91; /* 1+2+...+13 = 91 */
    static seq_t dp[92];
    static bool initialized = false;

    if (!initialized) {
        for (idx_t i = 0; i <= MAX_SUM; ++i) dp[i] = 0;
        dp[0] = 1;
        for (idx_t part = 1; part <= 13; ++part) {
            for (idx_t s = MAX_SUM; s >= part; --s) {
                dp[s] += dp[s - part];
            }
        }
        initialized = true;
    }

    if (n <= MAX_SUM) {
        return dp[(idx_t)n];
    }
    return 0;
}
