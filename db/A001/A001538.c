/*
 * A001538  a(n) = (12*n+1)*(12*n+11).
 * https://oeis.org/A001538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n+1)*(12*n+11)
 * Range:     u8 n <= 0, u16 n <= 20, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  seq_t m = (seq_t)n;
  return (12 * m + 1) * (12 * m + 11);
}
