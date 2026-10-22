/*
 * A004369  Binomial coefficient C(7n,n-1).
 * https://oeis.org/A004369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(7*n, n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq.h"

seq_t a(idx_t n) {
  idx_t m = 7 * n;
  idx_t k = n - 1;
  seq_t r = 1;
  idx_t i;
  for (i = 0; i < k; i++) {
    /* r = r * (m - i) / (i + 1), reduced to avoid intermediate overflow */
    seq_t d = (seq_t)(i + 1);
    seq_t g = gcd_seq(r, d);
    r /= g;
    d /= g;
    seq_t f = (seq_t)(m - i);
    f /= d;
    r *= f;
  }
  return r;
}
