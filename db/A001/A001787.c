/*
 * A001787  a(n) = n*2^(n-1).
 * https://oeis.org/A001787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*2^(n-1)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 467 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
  if (n == 0) return 0;
  return (seq_t)n * pow2(n - 1);
}
