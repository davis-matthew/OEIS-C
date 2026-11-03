/*
 * A005358  Number of low discrepancy sequences in base 5.
 * https://oeis.org/A005358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} (deg(p_i) - 1), where p_1, p_2, ... are the
 *            monic irreducible polynomials over GF(5) ordered by degree
 *            (Niederreiter's t-value). There are I(d) = (1/d) * Sum_{k|d}
 *            mu(k) * 5^(d/k) of them of degree d
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 79 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int a005358_mu(uint64_t k) {
  int r = 1;
  uint64_t p;
  for (p = 2; p * p <= k; p++) {
    if (k % p == 0) {
      k /= p;
      if (k % p == 0) return 0;
      r = -r;
    }
  }
  if (k > 1) r = -r;
  return r;
}

static uint64_t a005358_irr(uint64_t d) {
  __int128 s = 0;
  uint64_t k, j;
  for (k = 1; k <= d; k++) {
    if (d % k == 0) {
      int m = a005358_mu(k);
      if (m != 0) {
        __int128 p = 1;
        for (j = 0; j < d / k; j++) p *= 5;
        s += m * p;
      }
    }
  }
  return (uint64_t)(s / (__int128)d);
}

seq_t a(idx_t n) {
  seq_t result = 0;
  uint64_t remaining = n;
  uint64_t d;
  for (d = 1; remaining > 0 && d < 27; d++) {
    uint64_t c = a005358_irr(d);
    uint64_t take = c < remaining ? c : remaining;
    result += (seq_t)take * (seq_t)(d - 1);
    remaining -= take;
  }
  return result;
}
