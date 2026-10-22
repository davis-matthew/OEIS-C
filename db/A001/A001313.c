/*
 * A001313  Number of ways of making change for n cents using coins of 1, 2,
 *          5, 10, 20, 50 cents.
 * https://oeis.org/A001313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   1/((1-x)*(1-x^2)*(1-x^5)*(1-x^10)*(1-x^20)*(1-x^50))
 * Range:     u8 n <= 41, u16 n <= 197, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts from {1, 2, 5, 10, 20, 50} */

#include <stdlib.h>

static seq_t *dp = NULL;
static idx_t max_computed = 0;

seq_t a(idx_t n) {
  int coins[] = {1, 2, 5, 10, 20, 50};
  
  if (n <= max_computed && dp != NULL) {
    return dp[n];
  }
  
  idx_t new_max = (n > max_computed) ? n : max_computed;
  if (dp == NULL) {
    dp = (seq_t *)malloc((new_max + 1) * sizeof(seq_t));
  } else if (new_max > max_computed) {
    seq_t *new_dp = (seq_t *)realloc(dp, (new_max + 1) * sizeof(seq_t));
    dp = new_dp;
  }
  
  for (idx_t i = 0; i <= new_max; i++) {
    dp[i] = 0;
  }
  dp[0] = 1;
  
  for (int c = 0; c < 6; c++) {
    int coin = coins[c];
    for (idx_t i = coin; i <= new_max; i++) {
      dp[i] += dp[i - coin];
    }
  }
  
  max_computed = new_max;
  return dp[n];
}
