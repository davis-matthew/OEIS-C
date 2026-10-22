/*
 * A000359  Coefficients of iterated exponentials.
 * https://oeis.org/A000359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. -log(1+log(1+log(1+log(1+log(1-x))))) = u_5(x), where
 *            u_0(x) = x and u_{L}(x) = -log(1 - u_{L-1}(x)). If v = -log(1-u)
 *            then v' = u' + u*v', so in e.g.f. coefficients: v(1) = u(1),
 *            v(m+1) = u(m+1) + Sum_{k=1..m} binomial(m,k)*u(k)*v(m+1-k). a(n)
 *            = u_5(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"

static seq_t *U[6];
static idx_t cap = 0;

static void build(idx_t N) {
  idx_t m, k;
  int L;
  seq_t *B;
  for (L = 0; L <= 5; L++) U[L] = alloc_zero(N + 2);
  B = alloc_zero(N + 2);
  U[0][1] = 1;
  for (L = 1; L <= 5; L++) U[L][1] = U[L - 1][1];
  B[0] = 1;
  for (m = 1; m + 1 <= N; m++) {
    for (k = m; k >= 1; k--) B[k] += B[k - 1];
    for (L = 1; L <= 5; L++) {
      seq_t s = U[L - 1][m + 1];
      for (k = 1; k <= m; k++)
        s += B[k] * U[L - 1][k] * U[L][m + 1 - k];
      U[L][m + 1] = s;
    }
  }
  cap = N;
}

seq_t a(idx_t n) {
  if (n > cap) {
    idx_t N = 2 * cap;
    if (N < n) N = n;
    if (N < 32) N = 32;
    build(N);
  }
  return U[5][n];
}
