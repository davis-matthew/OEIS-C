/*
 * A000481  Stirling numbers of the second kind, S(n,5).
 * https://oeis.org/A000481
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = S(n,5) via S(n,k) = k*S(n-1,k) + S(n-1,k-1), with S(n,0)
 *            = 0 for n > 0 and row n=5: S(5,1..5) = 1, 15, 25, 10, 1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  seq_t s[6];
  idx_t i;
  int k;

  if (n < 5) return 0;

  s[0] = 0;
  s[1] = 1;
  s[2] = 15;
  s[3] = 25;
  s[4] = 10;
  s[5] = 1;

  for (i = 6; i <= n; i++) {
    for (k = 5; k >= 1; k--) {
      s[k] = (seq_t)k * s[k] + s[k - 1];
    }
  }

  return s[5];
}
