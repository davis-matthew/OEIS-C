/*
 * A003783  Coefficients of Jacobi Eisenstein series of index 1 and weight 8.
 * https://oeis.org/A003783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^n] t3^3*(t3^12 - 30*q*t3^8*P^4 + 240*q^2*t3^4*P^8 -
 *            224*q^3*P^12), where t3 = Sum_{k in Z} q^(k^2) and P =
 *            Sum_{k>=0} q^(k*(k+1)), so that theta_2^4 = 16*q*P^4
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   alloc_zero, mul_P, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/mul_P.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* in-place multiply by theta_3 (truncated to len terms) */
static void mul_t3(seq_t *s, idx_t len) {
  for (idx_t i = len; i-- > 0;) {
    seq_t acc = 0;
    for (idx_t k = 1; k * k <= i; k++) acc += s[i - k * k];
    s[i] += acc + acc;
  }
}

static seq_t *copy_series(const seq_t *s, idx_t len) {
  seq_t *r = alloc_zero(len);
  for (idx_t i = 0; i < len; i++) r[i] = s[i];
  return r;
}

static void compute(idx_t len) {
  int j;
  seq_t *R = alloc_zero(len);
  R[0] = 1;
  for (j = 0; j < 3; j++) mul_t3(R, len);          /* t3^3 */

  seq_t *T3 = copy_series(R, len);
  for (j = 0; j < 12; j++) mul_P(T3, len);         /* t3^3 P^12 */

  seq_t *U = copy_series(R, len);
  for (j = 0; j < 4; j++) mul_t3(U, len);          /* t3^7 */

  seq_t *T2 = copy_series(U, len);
  for (j = 0; j < 8; j++) mul_P(T2, len);          /* t3^7 P^8 */

  for (j = 0; j < 4; j++) mul_t3(U, len);          /* t3^11 */
  seq_t *T1 = copy_series(U, len);
  for (j = 0; j < 4; j++) mul_P(T1, len);          /* t3^11 P^4 */

  for (j = 0; j < 4; j++) mul_t3(U, len);          /* t3^15 */

  seq_t *res = alloc_zero(len);
  for (idx_t i = 0; i < len; i++) {
    seq_t pos = U[i];
    seq_t neg = 0;
    if (i >= 1) neg += T1[i - 1] * 30;
    if (i >= 2) pos += T2[i - 2] * 240;
    if (i >= 3) neg += T3[i - 3] * 224;
    res[i] = pos - neg;
  }

  release(R, len);
  release(T3, len);
  release(U, len);
  release(T2, len);
  release(T1, len);

  if (cache) release(cache, cache_len);
  cache = res;
  cache_len = len;
}

seq_t a(idx_t n) {
  if (n >= cache_len) {
    idx_t len = cache_len * 2;
    if (len < n + 1) len = n + 1;
    if (len < 64) len = 64;
    compute(len);
  }
  return cache[n];
}
