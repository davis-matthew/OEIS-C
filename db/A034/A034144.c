/*
 * A034144  Number of partitions of n into distinct parts from [1, 14].
 * https://oeis.org/A034144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1,2,…,14} whose sum is n
 *            (equivalently, number of partitions of n into distinct parts
 *            from 1 to 14)
 * Range:     u8 n <= 36, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* maximum possible sum with parts 1..14 is 105 */
    static seq_t dp[106] = {0};
    static int initialized = 0;

    if (!initialized) {
        initialized = 1;
        dp[0] = (seq_t)1;
        for (int k = 1; k <= 14; ++k) {
            for (int s = 105; s >= k; --s) {
                dp[s] += dp[s - k];
            }
        }
    }

    if (n <= (idx_t)105) {
        return dp[(size_t)n];
    } else {
        /* No subsets can sum to n > 105 */
        return (seq_t)0;
    }
}
