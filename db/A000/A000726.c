/*
 * A000726  Number of partitions of n in which no parts are multiples of 3.
 * https://oeis.org/A000726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = count of partitions of n with no parts divisible by 3
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 176, u64 n <= 591,
 *            u128 n <= 2112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= n; i++) {
        dp[i] = 0;
    }
    dp[0] = 1;
    
    for (idx_t k = 1; k <= n; k++) {
        if (k % 3 != 0) {
            for (idx_t i = k; i <= n; i++) {
                dp[i] += dp[i - k];
            }
        }
    }
    
    seq_t result = dp[n];
    free(dp);
    return result;
}
