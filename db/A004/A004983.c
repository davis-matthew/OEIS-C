/*
 * A004983  a(n) = (2^n/n!) * Product_{k=0..n-1} (4*k - 3).
 * https://oeis.org/A004983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*(4*n-7)*a(n-1)/n, a(0) = 1 (i.e. a(n) = (2^n/n!) *
 *            Product_{k=0..n-1} (4*k - 3))
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 13, i64 n <= 24, i128 n <= 46,
 *            big any n
 * Helpers:   a004983_gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004983_gcd_u64.h"

seq_t a(idx_t n) {
  seq_t result = 1;
  idx_t i;
  for (i = 1; i <= n; i++) {
    seq_t si = (seq_t)i;
    seq_t ap = result < 0 ? -result : result;
    uint64_t r = (uint64_t)(long long)(ap % si);
    uint64_t g = a004983_gcd_u64((uint64_t)i, r);
    seq_t sg = (seq_t)g;
    seq_t m = 2 * (4 * si - 7);
    /* i/g divides m exactly since the result is an integer and gcd(result/g, i/g) = 1 */
    result = (result / sg) * (m / (si / sg));
  }
  return result;
}
