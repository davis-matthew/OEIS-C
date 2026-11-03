/*
 * A001580  a(n) = 2^n + n^2.
 * https://oeis.org/A001580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + n^2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 920 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
  seq_t m = (seq_t)n;
  return pow2(n) + m * m;
}
