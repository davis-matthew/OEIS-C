/*
 * A001160  sigma_5(n), the sum of the 5th powers of the divisors of n.
 * https://oeis.org/A001160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sigma_5(n); multiplicative with a(p^e) = 1 + p^5 + p^10 +
 *            ... + p^(5e)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 83, u64 n <= 7085,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  seq_t result = 1;
  idx_t temp = n;
  idx_t p;

  if (n == 1) return 1;

  for (p = 2; p <= temp / p; p++) {
    if (temp % p == 0) {
      idx_t e = 0;
      idx_t i;
      seq_t ps = (seq_t)p;
      seq_t p5 = ps * ps * ps * ps * ps;
      seq_t sum = 1;
      seq_t term = 1;
      while (temp % p == 0) {
        temp /= p;
        e++;
      }
      for (i = 0; i < e; i++) {
        term = term * p5;
        sum = sum + term;
      }
      result = result * sum;
    }
  }

  if (temp > 1) {
    seq_t ps = (seq_t)temp;
    seq_t p5 = ps * ps * ps * ps * ps;
    result = result * (p5 + 1);
  }

  return result;
}
