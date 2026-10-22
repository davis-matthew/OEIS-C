/*
 * A004528  Ratios of successive terms are 1,2,2,2,3,4,4,4,5,6,6,6,7,...
 * https://oeis.org/A004528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1) * ratio(n-1), a(0) = 1, where ratio(i) = 2*floor(i/4) + 1 + (i mod 4 != 0) */

seq_t a(idx_t n) {
  if (n == 0) return 1;
  
  seq_t result = 1;
  for (idx_t i = 0; i < n; i++) {
    seq_t i_seq = (seq_t)i;
    seq_t ratio = 2 * (i_seq / 4) + 1 + ((i_seq & 3) != 0);
    result *= ratio;
  }
  return result;
}
