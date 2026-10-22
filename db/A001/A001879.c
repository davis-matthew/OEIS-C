/*
 * A001879  a(n) = (2n+2)!/(n!*2^(n+1)).
 * https://oeis.org/A001879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(2*n+1)!! where (2*n+1)!! = 1*3*5*...*(2*n+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  seq_t result = (seq_t)(n + 1);
  for (idx_t i = 1; i <= 2*n + 1; i += 2) {
    result *= (seq_t)i;
  }
  return result;
}
