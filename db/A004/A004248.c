/*
 * A004248  Array read by ascending antidiagonals: A(n, k) = k^n.
 * https://oeis.org/A004248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy, nice
 * Formula:   A(n, k) = k^n, read by antidiagonals
 * Range:     u8 n <= 39, u16 n <= 81, u32 n <= 194, u64 n <= 471,
 *            u128 n <= 1187, big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
  idx_t d = 0;
  idx_t antidiag_start = 0;
  
  while (antidiag_start + d + 1 <= n) {
    antidiag_start += d + 1;
    d++;
  }
  
  idx_t pos = n - antidiag_start;
  idx_t row = d - pos;
  idx_t col = pos;
  
  return a000248_pow((seq_t)col, row);
}
