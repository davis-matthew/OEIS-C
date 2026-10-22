/*
 * A001783  n-phi-torial, or phi-torial of n: Product k, 1 <= k <= n, k
 *          relatively prime to n.
 * https://oeis.org/A001783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Product of k where 1 <= k < n with gcd(k,n) = 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
  seq_t result = 1;
  for (idx_t k = 1; k < n; k++) {
    if (gcd(k, n) == 1) {
      result *= (seq_t)k;
    }
  }
  return result;
}
