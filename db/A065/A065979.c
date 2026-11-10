/*
 * A065979  Binomial transform of A002024.
 * https://oeis.org/A065979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n-1,k)*A002024(k+1), where
 *            A002024(i) = ceiling((sqrt(8*i+1)-1)/2)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
  idx_t m = n - 1;
  seq_t c = 1;
  seq_t total = 0;
  seq_t j = 1;
  for (idx_t k = 0; k <= m; k++) {
    seq_t i = (seq_t)k + 1;
    while (j * (j + 1) < 2 * i) j++;
    total += c * j;
    if (k < m) {
      c = c * (seq_t)(m - k) / (seq_t)(k + 1);
    }
  }
  return total;
}
