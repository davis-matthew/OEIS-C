/*
 * A003475  Expansion of Sum_{k>0} (-1)^(k+1) q^(k^2) /
 *          ((1-q)(1-q^3)(1-q^5)...(1-q^(2k-1))).
 * https://oeis.org/A003475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k>=1, k^2<=n} (-1)^(k+1) * [q^(n-k^2)]
 *            1/((1-q)(1-q^3)...(1-q^(2k-1))), computed incrementally;
 *            arithmetic mod 2^64 (the true values are tiny)
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
  uint64_t *c;
  uint64_t res = 0;
  idx_t i, k;
  if (n < 1) return 0;
  c = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
  if (!c) return 0;
  for (i = 0; i <= n; i++) c[i] = 0;
  c[0] = 1;
  for (k = 1; k * k <= n; k++) {
    idx_t lim = n - k * k;
    idx_t d = 2 * k - 1;
    for (i = d; i <= lim; i++) c[i] += c[i - d];
    if (k & 1) res += c[lim];
    else res -= c[lim];
  }
  free(c);
  return (seq_t)(long long)(int64_t)res;
}
