/*
 * A004101  Number of partitions of n of the form a_1*b_1^2 + a_2*b_2^2 + ...;
 *          number of semisimple rings with p^n elements for any prime p.
 * https://oeis.org/A004101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   G.f.: Product_{k>=1} 1/(1-x^k)^A046951(k) = Product_{d>=1}
 *            P(x^(d^2)), P(x) = Product_{m>=1} 1/(1-x^m); each P(x^(d^2)) is
 *            applied by dividing by Euler's pentagonal series Sum_j (-1)^j
 *            x^(d^2*j(3j-1)/2)
 * Range:     u8 n <= 14, u16 n <= 36, u32 n <= 104, u64 n <= 324,
 *            u128 n <= 1083, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *tab = NULL;
static idx_t tab_len = 0;

static void build(idx_t N) {
  seq_t *dp = alloc_zero(N);
  dp[0] = 1;
  for (idx_t d = 1; d * d < N; d++) {
    idx_t q = d * d;
    /* in-place division by prod_m (1 - x^(m q)) */
    for (idx_t i = q; i < N; i++) {
      seq_t pos = dp[i], neg = 0;
      for (idx_t k = 1;; k++) {
        idx_t g1 = q * (k * (3 * k - 1) / 2);
        if (g1 > i) break;
        idx_t g2 = q * (k * (3 * k + 1) / 2);
        if (k & 1) {
          pos += dp[i - g1];
          if (g2 <= i) pos += dp[i - g2];
        } else {
          neg += dp[i - g1];
          if (g2 <= i) neg += dp[i - g2];
        }
      }
      dp[i] = pos - neg;
    }
  }
  if (tab) release(tab, tab_len);
  tab = dp;
  tab_len = N;
}

seq_t a(idx_t n) {
  if (n >= tab_len) {
    idx_t N = n + 1;
    if (N < tab_len + tab_len / 2) N = tab_len + tab_len / 2;
    if (N < 64) N = 64;
    build(N);
  }
  return tab[n];
}
