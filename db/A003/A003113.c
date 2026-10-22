/*
 * A003113  Coefficients in expansion of permanent of infinite tridiagonal
 *          matrix shown below.
 * https://oeis.org/A003113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Sum_{m>=0} q^(m^2)*(1+q^m) / Prod_{k=1..m} (1-q^k) (sum of
 *            the two Rogers-Ramanujan series)
 * Range:     u8 n <= 32, u16 n <= 91, u32 n <= 287, u64 n <= 971,
 *            u128 n <= 1000, big any n
 * Helpers:   zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/zalloc_arr.h"

seq_t a(idx_t n) {
  seq_t *coeff = zalloc_arr(n + 1);
  seq_t *poly = zalloc_arr(n + 1);
  idx_t i, m;

  coeff[0] = coeff[0] + 2;
  poly[0] = poly[0] + 1;

  for (m = 1; m * m <= n; m++) {
    idx_t base = m * m;
    idx_t len = n - base + 1;
    /* poly now holds partitions into parts <= m (only first len terms needed) */
    for (i = m; i < len; i++) {
      poly[i] += poly[i - m];
    }
    for (i = 0; i < len; i++) {
      coeff[base + i] += poly[i];
      if (base + m + i <= n) {
        coeff[base + m + i] += poly[i];
      }
    }
  }

  seq_t result = coeff[n];
  free(poly);
  free(coeff);
  return result;
}
