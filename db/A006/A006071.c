/*
 * A006071  Maximal length of rook tour on an n X n board.
 * https://oeis.org/A006071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = n*(2*n^2-5)/3 + 2 for even n, n*(2*n^2-5)/3 + 1 for odd n
 *            >= 3 (Gardner); a(1) = 1 (a 1 X 1 board, where the general
 *            formula does not apply)
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 1394, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  if (n == 1) return (seq_t)1;
  seq_t nn = (seq_t)n;
  seq_t t = nn * (2 * nn * nn - 5) / 3;
  return t + ((n % 2 == 0) ? 2 : 1);
}
