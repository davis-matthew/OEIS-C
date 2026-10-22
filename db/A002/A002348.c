/*
 * A002348  Degree of rational Poncelet porism of n-gon.
 * https://oeis.org/A002348
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (n^2/8) * Prod_{p|n, p prime > 2} (1 - 1/p^2) = (1/8) *
 *            4^A007814(n) * A007434(A000265(n))
 * Range:     u8 n <= 16, u16 n <= 726, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  idx_t k = 0;
  idx_t m = n;
  while (m % 2 == 0) {
    k++;
    m /= 2;
  }

  if (m == 1) {
    /* n = 2^k, k >= 2: a(n) = 2^(2k-3) */
    seq_t r = 1;
    r <<= (2 * k - 3);
    return r;
  }

  /* Jordan J_2 of odd part m: prod p^(2(e-1)) * (p^2 - 1); first p^2-1 is divided by 8 */
  seq_t prod = 1;
  int divided = 0;
  idx_t temp = m;
  idx_t p;
  for (p = 3; p * p <= temp; p += 2) {
    if (temp % p == 0) {
      seq_t ps = (seq_t)p;
      seq_t f = (ps - 1) * (ps + 1);
      if (!divided) {
        f /= 8;
        divided = 1;
      }
      prod *= f;
      temp /= p;
      while (temp % p == 0) {
        prod *= ps * ps;
        temp /= p;
      }
    }
  }
  if (temp > 1) {
    seq_t ps = (seq_t)temp;
    seq_t f = (ps - 1) * (ps + 1);
    if (!divided) {
      f /= 8;
      divided = 1;
    }
    prod *= f;
  }

  /* multiply by 4^k */
  prod <<= (2 * k);
  return prod;
}
