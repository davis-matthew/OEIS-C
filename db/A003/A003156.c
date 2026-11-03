/*
 * A003156  A self-generating sequence (see Comments for definition).
 * https://oeis.org/A003156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A003159(n) - n, where A003159 lists the numbers whose
 *            binary expansion ends in an even number of zeros (found by a
 *            direct search)
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 856 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int a003156_even_val(uint64_t m) {
  int v = 0;
  while ((m & 1) == 0) {
    m >>= 1;
    v++;
  }
  return (v % 2) == 0;
}

seq_t a(idx_t n) {
  uint64_t m = 0;
  idx_t count = 0;
  while (count < n) {
    m++;
    if (a003156_even_val(m)) count++;
  }
  return (seq_t)2 * (seq_t)m - (seq_t)n;
}
