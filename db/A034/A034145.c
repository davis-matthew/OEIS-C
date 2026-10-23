/*
 * A034145  Number of partitions of n into distinct parts from [1, 15].
 * https://oeis.org/A034145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1,2,…,15} whose elements sum to n
 * Range:     u8 n <= 33, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t dp[121];
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        /* initialize DP once */
        for (int i = 0; i <= 120; ++i) dp[i] = 0;
        dp[0] = (seq_t)1;
        for (int k = 1; k <= 15; ++k) {
            for (int s = 120; s >= k; --s) {
                dp[s] = dp[s] + dp[s - k];
            }
        }
        initialized = true;
    }
    if (n <= 120) return dp[(int)n];
    return (seq_t)0;
}
