/*
 * A001297  Stirling numbers of the second kind S(n+3, n).
 * https://oeis.org/A001297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+1, 2) * binomial(n+3, 4)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 75, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  seq_t sn = (seq_t)n;
  seq_t n1 = sn + 1;
  seq_t n2 = sn + 2;
  seq_t n3 = sn + 3;
  
  seq_t bin2 = sn * n1 / 2;
  seq_t bin4 = sn * n1 * n2 * n3 / 24;
  
  return bin2 * bin4;
}
