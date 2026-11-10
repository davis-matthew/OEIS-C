/*
 * A055747  Expansion of Jacobi form of weight 12 and index 1 for the Niemeier
 *          lattice of type E_8^3 or D_16+E_8.
 * https://oeis.org/A055747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} A004009(k)*A003783(n-4k), i.e. g.f.
 *            = b(z)*c(z), b = A003783, c = A004009(z^4)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 22, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 34 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A003783_alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void A003783_release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static void A003783_mul_P(seq_t *s, idx_t len) {
  for (idx_t i = len; i-- > 0;) {
    seq_t acc = 0;
    for (idx_t k = 1; k * (k + 1) <= i; k++) acc += s[i - k * (k + 1)];
    s[i] += acc;
  }
}

static seq_t *A003783_cache = NULL;
static idx_t A003783_cache_len = 0;

static void A003783_mul_t3(seq_t *s, idx_t len) {
  for (idx_t i = len; i-- > 0;) {
    seq_t acc = 0;
    for (idx_t k = 1; k * k <= i; k++) acc += s[i - k * k];
    s[i] += acc + acc;
  }
}

static seq_t *A003783_copy_series(const seq_t *s, idx_t len) {
  seq_t *r = A003783_alloc_zero(len);
  for (idx_t i = 0; i < len; i++) r[i] = s[i];
  return r;
}

static void A003783_compute(idx_t len) {
  int j;
  seq_t *R = A003783_alloc_zero(len);
  R[0] = 1;
  for (j = 0; j < 3; j++) A003783_mul_t3(R, len);

  seq_t *T3 = A003783_copy_series(R, len);
  for (j = 0; j < 12; j++) A003783_mul_P(T3, len);

  seq_t *U = A003783_copy_series(R, len);
  for (j = 0; j < 4; j++) A003783_mul_t3(U, len);

  seq_t *T2 = A003783_copy_series(U, len);
  for (j = 0; j < 8; j++) A003783_mul_P(T2, len);

  for (j = 0; j < 4; j++) A003783_mul_t3(U, len);
  seq_t *T1 = A003783_copy_series(U, len);
  for (j = 0; j < 4; j++) A003783_mul_P(T1, len);

  for (j = 0; j < 4; j++) A003783_mul_t3(U, len);

  seq_t *res = A003783_alloc_zero(len);
  for (idx_t i = 0; i < len; i++) {
    seq_t pos = U[i];
    seq_t neg = 0;
    if (i >= 1) neg += T1[i - 1] * 30;
    if (i >= 2) pos += T2[i - 2] * 240;
    if (i >= 3) neg += T3[i - 3] * 224;
    res[i] = pos - neg;
  }

  A003783_release(R, len);
  A003783_release(T3, len);
  A003783_release(U, len);
  A003783_release(T2, len);
  A003783_release(T1, len);

  if (A003783_cache) A003783_release(A003783_cache, A003783_cache_len);
  A003783_cache = res;
  A003783_cache_len = len;
}

static seq_t A003783_a(idx_t n) {
  if (n >= A003783_cache_len) {
    idx_t len = A003783_cache_len * 2;
    if (len < n + 1) len = n + 1;
    if (len < 64) len = 64;
    A003783_compute(len);
  }
  return A003783_cache[n];
}

static seq_t A004009_a(idx_t n) {
    if (n == 0) return (seq_t)1;
    uint64_t sigma3 = 0;
    uint64_t n64 = (uint64_t)n;
    for (uint64_t i = 1; i * i <= n64; i++) {
        if (n64 % i == 0) {
            sigma3 += i * i * i;
            if (i != n64 / i) {
                uint64_t j = n64 / i;
                sigma3 += j * j * j;
            }
        }
    }
    return (seq_t)240 * (seq_t)sigma3;
}

seq_t a(idx_t n) {
    seq_t total = A003783_a(n);   /* k = 0 term; also fills cache up to n */
    for (idx_t k = 1; 4 * k <= n; k++) {
        total += A004009_a(k) * A003783_a(n - 4 * k);
    }
    return total;
}
