/*
 * A002635  Number of partitions of n into 4 squares.
 * https://oeis.org/A002635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = number of (a,b,c,d) with a>=b>=c>=d>=0 and
 *            a^2+b^2+c^2+d^2 = n; computed by a direct search over d<=c<=b,
 *            with a found by integer square root
 * Range:     u8 n <= 4049, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a2635_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a2635_isqrt.h"

seq_t a(idx_t n) {
  seq_t count = 0;
  uint64_t N = (uint64_t)n;
  uint64_t d, c, b;
  for (d = 0; 4 * d * d <= N; d++) {
    for (c = d; d * d + 3 * c * c <= N; c++) {
      for (b = c; d * d + c * c + 2 * b * b <= N; b++) {
        uint64_t r = N - d * d - c * c - b * b;
        uint64_t s = a2635_isqrt(r);
        if (s * s == r) count++;
      }
    }
  }
  return count;
}
