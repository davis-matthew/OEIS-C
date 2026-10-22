/*
 * A000537  Sum of first n cubes; or n-th triangular number squared.
 * https://oeis.org/A000537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = (n*(n+1)/2)^2
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 361, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
  seq_t n_seq = (seq_t)n;
  seq_t triangular = n_seq * (n_seq + 1) / 2;
  return triangular * triangular;
}
