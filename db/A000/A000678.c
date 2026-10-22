/*
 * A000678  Number of carbon (rooted) trees with n carbon atoms = unordered
 *          4-tuples of ternary trees.
 * https://oeis.org/A000678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f. A(x) = x*Z(S4; B(x)), B(x) = 1 + x*Z(S3; B(x)) (A000598).
 *            Z(Sk; B) is computed exactly as a knapsack over tree weights w,
 *            adding t copies chosen from b(w) trees in C(b(w)+t-1, t) ways;
 *            b(w+1) = [x^w] Z(S3; B), a(n+1) = [x^n] Z(S4; B), a(0) = 0
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 49, u128 n <= 60,
 *            big any n
 * Helpers:   a678_mc, alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/a678_mc.h"

static seq_t *a678_D[5];
static idx_t a678_cap = 0;

static void a678_build(idx_t L) {
  int j, t;
  idx_t w, s;
  for (j = 0; j <= 4; j++) {
    if (a678_D[j]) free(a678_D[j]);
    a678_D[j] = alloc_zero(L);
    a678_D[j][0] = 1;
  }
  for (w = 1; w < L; w++) {
    seq_t bw = a678_D[3][w - 1];
    seq_t m[5];
    for (t = 0; t <= 4; t++) m[t] = 0;
    for (t = 1; t <= 4 && (idx_t)t * w <= L - 1; t++) m[t] = a678_mc(bw, t);
    for (s = L - 1; s >= w; s--) {
      for (j = 4; j >= 1; j--) {
        for (t = 1; t <= j && (idx_t)t * w <= s; t++) {
          a678_D[j][s] += m[t] * a678_D[j - t][s - (idx_t)t * w];
        }
      }
    }
  }
  a678_cap = L;
}

seq_t a(idx_t n) {
  idx_t S;
  if (n == 0) return 0;
  S = n - 1;
  if (S >= a678_cap) {
    idx_t L = 2 * a678_cap;
    if (L < S + 1) L = S + 1;
    if (L < 64) L = 64;
    a678_build(L);
  }
  return a678_D[4][S];
}
