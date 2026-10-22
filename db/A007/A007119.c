/*
 * A007119  Expansion of (1-x)^sin(x).
 * https://oeis.org/A007119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   E.g.f. A(x) = (1-x)^sin(x) = exp(g(x)), g(x) = sin(x)*log(1-x).
 *            With b(m) = m! [x^m] g(x) = -Sum_{s odd, s<m} (-1)^((s-1)/2) *
 *            binomial(m,s) * (m-s-1)!, a(0) = 1, a(n) = Sum_{k=0..n-1}
 *            binomial(n-1,k) * b(k+1) * a(n-1-k)
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 15, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   alloc_zero, from_i128, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/from_i128.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

typedef unsigned __int128 a007119_u128;

/* Fixed-width builds: exact arithmetic modulo 2^128 (only + - *), so every
   result that fits in seq_t is recovered exactly despite large intermediates. */
static seq_t a007119_fixed(idx_t n) {
  a007119_u128 *A = (a007119_u128 *)malloc((n + 2) * sizeof(a007119_u128));
  a007119_u128 *B = (a007119_u128 *)malloc((n + 2) * sizeof(a007119_u128));
  a007119_u128 *F = (a007119_u128 *)malloc((n + 2) * sizeof(a007119_u128));
  a007119_u128 *row = (a007119_u128 *)malloc((n + 2) * sizeof(a007119_u128));
  a007119_u128 *prev = (a007119_u128 *)malloc((n + 2) * sizeof(a007119_u128));
  idx_t i, m, k, s;
  F[0] = 1;
  for (i = 1; i <= n + 1; i++) F[i] = F[i - 1] * (a007119_u128)i;
  for (i = 0; i <= n + 1; i++) row[i] = 0;
  row[0] = 1;
  A[0] = 1;
  B[0] = 0;
  for (m = 1; m <= n; m++) {
    for (k = 0; k < m; k++) prev[k] = row[k];
    for (k = m; k >= 1; k--) row[k] = row[k] + row[k - 1];
    B[m] = 0;
    for (s = 1; s < m; s += 2) {
      a007119_u128 t = row[s] * F[m - s - 1];
      if (((s - 1) / 2) % 2 == 0) B[m] -= t; else B[m] += t;
    }
    A[m] = 0;
    for (k = 0; k < m; k++) A[m] += prev[k] * B[k + 1] * A[m - 1 - k];
  }
  {
    seq_t r = from_i128((__int128)A[n]);
    free(A); free(B); free(F); free(row); free(prev);
    return r;
  }
}

static seq_t *a007119_memo = NULL;
static idx_t a007119_cap = 0;

static void a007119_big_fill(idx_t N) {
  seq_t *A = alloc_zero(N + 2);
  seq_t *B = alloc_zero(N + 2);
  seq_t *F = alloc_zero(N + 2);
  seq_t *row = alloc_zero(N + 2);
  seq_t *prev = alloc_zero(N + 2);
  idx_t i, m, k, s;
  F[0] = 1;
  for (i = 1; i <= N + 1; i++) F[i] = F[i - 1] * (seq_t)i;
  row[0] = 1;
  A[0] = 1;
  B[0] = 0;
  for (m = 1; m <= N; m++) {
    for (k = 0; k < m; k++) prev[k] = row[k];
    for (k = m; k >= 1; k--) row[k] = row[k] + row[k - 1];
    B[m] = 0;
    for (s = 1; s < m; s += 2) {
      seq_t t = row[s] * F[m - s - 1];
      if (((s - 1) / 2) % 2 == 0) B[m] -= t; else B[m] += t;
    }
    A[m] = 0;
    for (k = 0; k < m; k++) A[m] += prev[k] * B[k + 1] * A[m - 1 - k];
  }
  release(B, N + 2);
  release(F, N + 2);
  release(row, N + 2);
  release(prev, N + 2);
  if (a007119_memo) release(a007119_memo, a007119_cap + 2);
  a007119_memo = A;
  a007119_cap = N;
}

seq_t a(idx_t n) {
  if (seq_is_fixed_width()) return a007119_fixed(n);
  if (a007119_memo == NULL || n > a007119_cap) {
    idx_t N = n;
    if (a007119_memo != NULL && N < 2 * a007119_cap) N = 2 * a007119_cap;
    if (N < 32) N = 32;
    a007119_big_fill(N);
  }
  return a007119_memo[n];
}
