/*
 * A002799  Number of 4-line partitions of n (i.e., planar partitions of n
 *          with at most 4 lines).
 * https://oeis.org/A002799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Euler transform of c = 1, 2, 3, 4, 4, 4, ...: G.f.
 *            Product_{k>=1} 1/(1-x^k)^c(k). Fixed-width build: multiply in
 *            each factor 1/(1-x^k) by f[i] += f[i-k]. Big build: a(0) = 1,
 *            n*a(n) = Sum_{k=1..n} b(k)*a(n-k), where b(k) = Sum_{d|k} d*c(d)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 55, u64 n <= 157, u128 n <= 489,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static int a2799_fixed_flag = -1;

static int a2799_is_fixed(void) {
  if (a2799_fixed_flag < 0) {
    seq_t x = 1;
    int i;
    a2799_fixed_flag = 0;
    for (i = 0; i < 300; i++) {
      x = x + x;
      if (x == 0) { a2799_fixed_flag = 1; break; }
    }
  }
  return a2799_fixed_flag;
}

/* fixed-width cache */
static seq_t *a2799_F = NULL;
static idx_t a2799_Flen = 0;

static seq_t a2799_fixed_calc(idx_t n) {
  if (n >= a2799_Flen) {
    idx_t N = a2799_Flen * 2;
    idx_t i, k;
    if (N < n + 1) N = n + 1;
    if (N < 64) N = 64;
    seq_t *f = (seq_t *)calloc((size_t)N, sizeof(seq_t));
    f[0] = 1;
    for (k = 1; k < N; k++) {
      int c = k <= 3 ? (int)k : 4, r;
      for (r = 0; r < c; r++)
        for (i = k; i < N; i++) f[i] += f[i - k];
    }
    free(a2799_F);
    a2799_F = f;
    a2799_Flen = N;
  }
  return a2799_F[n];
}

/* big cache */
static seq_t *a2799_A = NULL;
static idx_t a2799_Alen = 0;
static idx_t a2799_Acap = 0;
static uint64_t *a2799_B = NULL;
static idx_t a2799_Bcap = 0;

static void a2799_make_b(idx_t cap) {
  idx_t d, m;
  uint64_t *b = (uint64_t *)calloc((size_t)cap, sizeof(uint64_t));
  for (d = 1; d < cap; d++) {
    uint64_t c = d <= 3 ? d : 4;
    for (m = d; m < cap; m += d) b[m] += c * d;
  }
  free(a2799_B);
  a2799_B = b;
  a2799_Bcap = cap;
}

static seq_t a2799_big_calc(idx_t n) {
  idx_t m, k;
  if (n >= a2799_Acap) {
    idx_t cap = a2799_Acap * 2;
    if (cap < n + 1) cap = n + 1;
    if (cap < 64) cap = 64;
    seq_t *na = (seq_t *)calloc((size_t)cap, sizeof(seq_t));
    for (m = 0; m < a2799_Alen; m++) na[m] = a2799_A[m];
    free(a2799_A);
    a2799_A = na;
    a2799_Acap = cap;
    a2799_make_b(cap);
  }
  if (a2799_Alen == 0) {
    a2799_A[0] = 1;
    a2799_Alen = 1;
  }
  for (m = a2799_Alen; m <= n; m++) {
    seq_t s = 0;
    for (k = 1; k <= m; k++) s += a2799_A[m - k] * a2799_B[k];
    a2799_A[m] = s / m;
    a2799_Alen = m + 1;
  }
  return a2799_A[n];
}

seq_t a(idx_t n) {
  if (a2799_is_fixed()) return a2799_fixed_calc(n);
  return a2799_big_calc(n);
}
