/*
 * A000461  Concatenate n n times.
 * https://oeis.org/A000461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n repeated n times (concatenated as a decimal number)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  // Compute the number of digits in n
  unsigned d = 1;
  idx_t temp = n;
  while (temp >= 10) {
    d++;
    temp /= 10;
  }
  
  // Compute 10^d
  seq_t base = 1;
  for (unsigned i = 0; i < d; i++) {
    base *= 10;
  }
  
  // Concatenate n exactly n times
  seq_t result = 0;
  seq_t n_seq = (seq_t)n;
  for (idx_t i = 0; i < n; i++) {
    result = result * base + n_seq;
  }
  
  return result;
}
