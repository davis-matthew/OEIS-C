/*
 * A003408  a(n) = binomial(3n+6, n).
 * https://oeis.org/A003408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3n+6, n), computed multiplicatively: r_i =
 *            r_{i-1} * (2n+6+i) / i
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   a003408_gcd_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a003408_gcd_u.h"

seq_t a(idx_t n) {
  seq_t r = 1;
  for (idx_t i = 1; i <= n; i++) {
    uint64_t rm = (uint64_t)(r % (seq_t)i);
    uint64_t g = a003408_gcd_u(rm, (uint64_t)i);
    if (g == 0) g = i;
    uint64_t d = (uint64_t)i / g;
    seq_t num = (seq_t)n * 2 + 6 + (seq_t)i;
    r /= (seq_t)g;
    r *= num / (seq_t)d;
  }
  return r;
}
