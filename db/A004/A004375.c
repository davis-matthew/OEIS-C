/*
 * A004375  Binomial coefficient C(7n,n-7).
 * https://oeis.org/A004375
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(7n, n-7), computed multiplicatively with gcd reduction
 *            to avoid intermediate overflow
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 13, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Helpers:   a004375_gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004375_gcd_u64.h"

seq_t a(idx_t n) {
  idx_t m = 7 * n;
  idx_t k = n - 7;
  if (k > m - k) k = m - k;
  seq_t r = 1;
  idx_t i;
  for (i = 1; i <= k; i++) {
    /* r = r * (m - k + i) / i, exactly, keeping intermediates <= the result */
    uint64_t rem = (uint64_t)(r % (seq_t)i);
    uint64_t g = a004375_gcd_u64(rem, (uint64_t)i);
    uint64_t d = (uint64_t)i / g;
    r /= (seq_t)g;
    seq_t num = (seq_t)(m - k + i);
    num /= (seq_t)d;
    r *= num;
  }
  return r;
}
