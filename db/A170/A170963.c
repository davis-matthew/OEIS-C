/*
 * A170963  Expansion of Product_{i=1..m} (1 + x^(4*i-1)) for m = 10.
 * https://oeis.org/A170963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = coefficient of x^n in ∏_{i=1}^{10} (1 + x^{4i-1})
 * Range:     u8 n <= 210, u16 n <= 210, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t dp[211];
    static bool initialized = false;
    if (!initialized) {
        /* Initialise DP table for sums up to 210 */
        for (idx_t i = 0; i < 211; ++i) dp[i] = 0;
        dp[0] = 1;
        const idx_t exps[10] = {3, 7, 11, 15, 19, 23, 27, 31, 35, 39};
        for (int k = 0; k < 10; ++k) {
            idx_t e = exps[k];
            for (idx_t i = 210; i >= e; --i) {
                dp[i] += dp[i - e];
            }
        }
        initialized = true;
    }
    if (n > 210) return 0;
    return dp[n];
}
