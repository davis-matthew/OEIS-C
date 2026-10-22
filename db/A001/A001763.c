/*
 * A001763  a(n) = (3n+3)!/(2n+3)!.
 * https://oeis.org/A001763
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3n+3)!/(2n+3)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  seq_t result = 1;
  for (idx_t i = 1; i <= n; i++) {
    result *= (seq_t)(2*n + 3 + i);
  }
  return result;
}
