/*
 * A002456  Joffe's central differences of 0, A241171(n,n-1).
 * https://oeis.org/A002456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n)! * (n-1) / (6 * 2^n), recurrence: a(n) = a(n-1) * n
 *            * (2n-1) * (n-1) / (n-2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  if (n == 1) return 0;
  if (n == 2) return 1;

  seq_t result = 1;

  for (idx_t i = 3; i <= n; i++) {
    result = result * (seq_t)i * (seq_t)(2 * i - 1) * (seq_t)(i - 1) / (seq_t)(i - 2);
  }

  return result;
}
