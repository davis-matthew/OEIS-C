/*
 * A002999  Expansion of (1 + x*exp(x))^2.
 * https://oeis.org/A002999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = (n^2 - n)*2^n/4 + 2*n = n*(n-1)*2^(n-2) + 2*n
 *            for n >= 2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  if (n == 0) return 1;
  if (n == 1) return 2;

  seq_t p = 1;
  idx_t i;
  for (i = 2; i < n; i++) p *= 2;

  seq_t m = (seq_t)n;
  return m * (m - 1) * p + 2 * m;
}
