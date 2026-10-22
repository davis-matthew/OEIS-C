/*
 * A006504  Coefficient of x^4 in (1-x-x^2)^(-n).
 * https://oeis.org/A006504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+3)*(n+14)/24
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 261, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  seq_t n_seq = (seq_t)n;
  return n_seq * (n_seq + 1) * (n_seq + 3) * (n_seq + 14) / 24;
}
