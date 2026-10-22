/*
 * A005448  Centered triangular numbers: a(n) = 3*n*(n-1)/2 + 1.
 * https://oeis.org/A005448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3*n*(n-1)/2 + 1
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
  seq_t m = (seq_t)n;
  return 3 * (m * (m - 1) / 2) + 1;
}
