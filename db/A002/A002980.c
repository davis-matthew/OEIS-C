/*
 * A002980  E.g.f: 1/(1 - sin(x) + sin(x)^2 - sin(x)^3).
 * https://oeis.org/A002980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   E.g.f. 1/(1-s+s^2-s^3) = (1+s)/(1-s^4) = Sum_{m>=0} c(m)*s^m
 *            with s = sin(x), c(m) = 1 if m mod 4 is 0 or 1, else 0. On
 *            functions of s, D^2 = (1-s^2)*d^2/ds^2 - s*d/ds, so it maps Sum
 *            w_k s^k to Sum w'_k s^k with w'_k = (k+1)*(k+2)*w_{k+2} -
 *            k^2*w_k. Start with w = c and apply this j times: then...
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   A_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A_grow.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

static void build(idx_t N) {
  idx_t L = N + 2;
  seq_t *w = A_grow(NULL, 0, L);
  seq_t *r = A_grow(memo, memo_len, N + 1);
  idx_t m, j, k, top;
  for (m = 0; m < L; m++) w[m] = (seq_t)((m % 4 < 2) ? 1 : 0);
  top = L;
  for (j = 0; 2 * j <= N; j++) {
    r[2 * j] = w[0];
    if (2 * j + 1 <= N) r[2 * j + 1] = w[1];
    if (top < 3) break;
    for (k = 0; k + 2 < top; k++) {
      w[k] = (seq_t)(k + 2) * (seq_t)(k + 1) * w[k + 2] - (seq_t)k * (seq_t)k * w[k];
    }
    top -= 2;
  }
  free(w);
  memo = r;
  memo_len = N + 1;
}

seq_t a(idx_t n) {
  if (n >= memo_len) {
    idx_t N = n;
    if (N < 2 * memo_len) N = 2 * memo_len;
    build(N);
  }
  return memo[n];
}
