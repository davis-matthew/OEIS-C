/*
 * A001812  Coefficients of Laguerre polynomials.
 * https://oeis.org/A001812
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,5) * n!/5! = ((n!/5!)^2)/(n-5)!, n >= 5
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  if (n < 5) return 0;

  /* binomial(n,5), computed multiplicatively so every division is exact */
  seq_t c = 1;
  idx_t i;
  for (i = 1; i <= 5; i++) {
    c = c * (seq_t)(n - 5 + i) / (seq_t)i;
  }

  /* n!/5! = 6*7*...*n */
  seq_t prod = 1;
  for (i = 6; i <= n; i++) {
    prod *= (seq_t)i;
  }

  return c * prod;
}
