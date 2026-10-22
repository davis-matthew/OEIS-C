/*
 * A001372  Number of unlabeled mappings (or mapping patterns) from n points
 *          to themselves; number of unlabeled endofunctions.
 * https://oeis.org/A001372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n) = (1/n)*Sum_{k=1..n} B(k)*a(n-k), a(0) = 1, where B(n) =
 *            Sum_{d|n} (n/d)*M(d), M(n) = Sum_{j=1..n} j*t(j)*S(n-j), S =
 *            1/(1 - T(x)), T(x) = Sum t(n) x^n the rooted trees A000081
 *            (t(m+1) = (1/m)*Sum_{k=1..m} (Sum_{d|k} d*t(d))*t(m-k+1), t(1) =
 *            1); i.e. a = Product_{k>=1} 1/(1 - T(x^k)), the Eu...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Helpers:   a1372_is_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a1372_is_fixed.h"

#define A1372_LIM 10002

static seq_t a1372_T[A1372_LIM], a1372_Bt[A1372_LIM], a1372_S[A1372_LIM];
static seq_t a1372_M[A1372_LIM], a1372_Bb[A1372_LIM], a1372_A[A1372_LIM];
static seq_t a1372_F[A1372_LIM];
static idx_t a1372_done = 0;
static int a1372_mode = -1;

static void a1372_extend(idx_t n, int fixed) {
  for (idx_t i = a1372_done; i <= n; i++) {
    if (i == 0) {
      a1372_T[0] = 0; a1372_Bt[0] = 0; a1372_S[0] = 1;
      a1372_M[0] = 0; a1372_Bb[0] = 0; a1372_A[0] = 1;
      continue;
    }
    if (i == 1) {
      a1372_T[1] = 1;
    } else {
      seq_t s = 0;
      for (idx_t k = 1; k < i; k++) s += a1372_Bt[k] * a1372_T[i - k];
      a1372_T[i] = s / (seq_t)(i - 1);
    }
    {
      seq_t s = 0;
      for (idx_t d = 1; d <= i; d++)
        if (i % d == 0) s += (seq_t)d * a1372_T[d];
      a1372_Bt[i] = s;
    }
    {
      seq_t s = 0, m = 0;
      for (idx_t j = 1; j <= i; j++) {
        seq_t p = a1372_T[j] * a1372_S[i - j];
        s += p;
        m += (seq_t)j * p;
      }
      a1372_S[i] = s;
      a1372_M[i] = m;
    }
    if (!fixed) {
      seq_t s = 0;
      for (idx_t d = 1; d <= i; d++)
        if (i % d == 0) s += (seq_t)(i / d) * a1372_M[d];
      a1372_Bb[i] = s;
      seq_t r = 0;
      for (idx_t k = 1; k <= i; k++) r += a1372_Bb[k] * a1372_A[i - k];
      a1372_A[i] = r / (seq_t)i;
    }
  }
  if (n + 1 > a1372_done) a1372_done = n + 1;
}

seq_t a(idx_t n) {
  if (a1372_mode < 0) a1372_mode = a1372_is_fixed();
  int fixed = a1372_mode;
  if (fixed) {
    if (n > 400) return 0; /* far beyond any fixed-width value */
  } else if (n >= A1372_LIM - 1) {
    return 0;
  }
  if (n >= a1372_done) a1372_extend(n, fixed);
  if (!fixed) return a1372_A[n];

  /* fixed width: A = Product_{k>=1} S(x^k), division-free */
  idx_t N = n;
  for (idx_t i = 0; i <= N; i++) a1372_F[i] = a1372_S[i];
  for (idx_t k = 2; k <= N; k++) {
    for (idx_t i = N; i >= k; i--) {
      seq_t s = 0;
      idx_t j = 1;
      for (idx_t p = k; p <= i; p += k, j++) s += a1372_S[j] * a1372_F[i - p];
      a1372_F[i] += s;
    }
  }
  return a1372_F[n];
}
