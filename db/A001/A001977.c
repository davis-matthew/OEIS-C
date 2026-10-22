/*
 * A001977  Number of partitions of 3n into n parts from the set {0, 1, ...,
 *          6} (repetitions admissible).
 * https://oeis.org/A001977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   partitions of 3n into exactly n parts from {0,1,2,3,4,5,6};
 *            dynamic programming counting partitions by size and sum
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 351, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    idx_t target_sum = 3 * n;
    
    /* dp[k][s] = number of ways to partition s into exactly k parts,
       where each part is from {0,1,2,3,4,5,6}. */
    seq_t** dp = (seq_t**)malloc((n + 1) * sizeof(seq_t*));
    for (idx_t k = 0; k <= n; k++) {
        dp[k] = (seq_t*)calloc(target_sum + 1, sizeof(seq_t));
    }
    
    dp[0][0] = 1;
    
    /* For each part value, update the DP table. The recurrence
       dp[k][s] += dp[k-1][s-part] ensures we count partitions where
       parts are non-decreasing (no duplicates). */
    for (int part = 0; part <= 6; part++) {
        for (idx_t k = 1; k <= n; k++) {
            for (idx_t s = (idx_t)part; s <= target_sum; s++) {
                dp[k][s] += dp[k - 1][s - (idx_t)part];
            }
        }
    }
    
    seq_t result = dp[n][target_sum];
    
    for (idx_t k = 0; k <= n; k++) {
        free(dp[k]);
    }
    free(dp);
    
    return result;
}
