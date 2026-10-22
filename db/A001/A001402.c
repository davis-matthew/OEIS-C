/*
 * A001402  Number of partitions of n into at most 6 parts.
 * https://oeis.org/A001402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 78, u32 n <= 809, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Partitions of n into at most 6 parts; computed via DP over parts 1..6 */
seq_t a(idx_t n) {
  static seq_t *dp = NULL;
  static idx_t dp_capacity = 0;
  
  if (n >= dp_capacity) {
    idx_t new_capacity = (n + 1) * 2;
    seq_t *new_dp = (seq_t *)malloc(new_capacity * sizeof(seq_t));
    if (!new_dp) return 0;
    free(dp);
    dp = new_dp;
    dp_capacity = new_capacity;
  }
  
  for (idx_t i = 0; i <= n; i++) {
    dp[i] = 0;
  }
  dp[0] = 1;
  
  for (int part = 1; part <= 6; part++) {
    for (idx_t i = part; i <= n; i++) {
      dp[i] += dp[i - part];
    }
  }
  
  return dp[n];
}
