/*
 * A004762  Numbers whose binary expansion does not begin 100.
 * https://oeis.org/A004762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(1) = 0, a(2) = 1, a(3) = 2, a(4) = 3, a(2^(m+1) + 2^m + k + 2)
 *            = 2^(m+2) + 2^m + k
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  if (n == 1) return 0;
  if (n == 2) return 1;
  if (n == 3) return 2;
  if (n == 4) return 3;
  
  idx_t n_minus_2 = n - 2;
  idx_t m = 0;
  
  while (m < 62 && (n_minus_2 >> (m + 1)) >= 3) {
    m++;
  }
  
  seq_t pow2_m1 = (seq_t)(1ull << (m + 1));
  return pow2_m1 + (seq_t)n_minus_2;
}
