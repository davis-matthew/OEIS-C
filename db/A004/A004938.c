/*
 * A004938  a(n) = round(n*phi^3), where phi is the golden ratio, A001622.
 * https://oeis.org/A004938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*phi^3), where phi^3 = 2 + sqrt(5)
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  // phi^3 ≈ 4.2360679774997896964091736687312762...
  // Using rational approximation: 4236067977500 / 10^12
  seq_t numerator = (seq_t)4236067977500;
  seq_t denominator = (seq_t)1000000000000;
  seq_t n_seq = (seq_t)n;
  
  return (n_seq * numerator + denominator / 2) / denominator;
}
