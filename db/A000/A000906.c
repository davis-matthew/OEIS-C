/*
 * A000906  Exponential generating function: 2*(1+3*x)/(1-2*x)^(7/2).
 * https://oeis.org/A000906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n+3)!!/3 * 2*(n+1) = 2*(n+1) * prod_{k=2..n+1} (2k+1)
 *            (equivalent to a(n) = (2n+3)!/(3!*n!*2^(n-1)))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  seq_t r = (seq_t)2 * ((seq_t)n + 1);
  for (idx_t k = 2; k <= n + 1; k++) {
    r *= (seq_t)2 * (seq_t)k + 1;
  }
  return r;
}
