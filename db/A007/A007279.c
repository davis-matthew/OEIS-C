/*
 * A007279  Number of partitions of n into partition numbers.
 * https://oeis.org/A007279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: 1/Product_{k>=1} (1-q^A000041(k)) Count partitions of n
 *            using only partition numbers as parts
 * Range:     u8 n <= 23, u16 n <= 80, u32 n <= 386, u64 n <= 2741,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


seq_t a(idx_t n) {
    // Compute partition numbers p(k) for k >= 1
    seq_t *partition_counts = (seq_t *)calloc(n + 1, sizeof(seq_t));
    partition_counts[0] = 1;
    
    for (idx_t i = 1; i <= n; i++) {
        for (idx_t j = i; j <= n; j++) {
            partition_counts[j] += partition_counts[j - i];
        }
    }
    
    // Collect partition numbers p(k) where p(k) <= n
    seq_t *partition_nums = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    idx_t num_partition_nums = 0;
    
    for (idx_t k = 1; k <= n; k++) {
        if (partition_counts[k] <= (seq_t)n) {
            partition_nums[num_partition_nums++] = partition_counts[k];
        } else {
            break;
        }
    }
    
    // Count partitions of n using partition numbers
    seq_t *dp = (seq_t *)calloc(n + 1, sizeof(seq_t));
    dp[0] = 1;
    
    for (idx_t i = 0; i < num_partition_nums; i++) {
        seq_t part_num = partition_nums[i];
        idx_t part_num_idx = (idx_t)part_num;
        for (idx_t j = part_num_idx; j <= n; j++) {
            dp[j] += dp[j - part_num_idx];
        }
    }
    
    seq_t result = dp[n];
    free(partition_counts);
    free(partition_nums);
    free(dp);
    return result;
}
