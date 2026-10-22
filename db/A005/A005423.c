/*
 * A005423  A finite sequence associated with the Lie algebra A_6.
 * https://oeis.org/A005423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   finite sequence of 14 terms associated with Lie algebra A_6
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  static const seq_t values[] = {
    1, 2, 3, 4, 5, 6, 8, 10, 11, 14, 20, 21, 35, 56
  };
  
  if (n < 14) {
    return values[n];
  }
  return 0;
}
