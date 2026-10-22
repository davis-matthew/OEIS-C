/*
 * A000707  Number of permutations of [1,2,...,n] with n-1 inversions.
 * https://oeis.org/A000707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{j} (-1)^j * binomial(2n-2-j(3j-1)/2, n-1), summed
 *            over all integers j with j(3j-1)/2 <= n-1. This comes from the
 *            coefficient of q^(n-1) in Prod_{k=1..n} (1-q^k)/(1-q), using
 *            Euler's pentagonal theorem. For fixed-width seq_t the code
 *            instead uses the equivalent O(n^2) DP over submultis...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   a000707_binom, a000707_dp, a000707_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a000707_binom.h"
#include "../utils/a000707_dp.h"
#include "../utils/a000707_is_big.h"

seq_t a(idx_t n) {
  if (n < 1) return 0;
  if (!a000707_is_big()) return a000707_dp(n);
  idx_t k = n - 1;
  seq_t pos = a000707_binom(2 * k, k);
  seq_t neg = 0;
  for (idx_t j = 1;; j++) {
    idx_t g1 = j * (3 * j - 1) / 2;
    if (g1 > k) break;
    seq_t t = a000707_binom(2 * k - g1, k);
    idx_t g2 = j * (3 * j + 1) / 2;
    if (g2 <= k) t += a000707_binom(2 * k - g2, k);
    if (j % 2 == 1) neg += t; else pos += t;
  }
  return pos - neg;
}
