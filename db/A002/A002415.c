/*
 * A002415  4-dimensional pyramidal numbers: a(n) = n^2*(n^2-1)/12.
 * https://oeis.org/A002415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n^2*(n^2-1)/12
 * Range:     u8 n <= 7, u16 n <= 29, u32 n <= 322, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  seq_t nn = (seq_t)n;
  seq_t nn2 = nn * nn;
  return nn2 * (nn2 - 1) / 12;
}
