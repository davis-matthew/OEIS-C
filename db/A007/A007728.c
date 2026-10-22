/*
 * A007728  5th binary partition function.
 * https://oeis.org/A007728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   G.f.: Product_{k>=0} (1 - x^(5*2^k))/(1 - x^(2^k))
 * Range:     u8 n <= 139, u16 n <= 8851, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
  seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
  if (!dp) return 0;
  
  for (idx_t i = 0; i <= n; i++) {
    dp[i] = 0;
  }
  dp[0] = 1;
  
  idx_t power = 1;
  while (power <= n) {
    for (idx_t i = n; i >= power; i--) {
      for (int c = 1; c <= 4; c++) {
        if (power > i / c) break;
        dp[i] += dp[i - (idx_t)c * power];
      }
    }
    if (power > n >> 1) break;
    power <<= 1;
  }
  
  seq_t result = dp[n];
  free(dp);
  return result;
}
