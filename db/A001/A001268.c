/*
 * A001268  One-half the number of permutations of length n with exactly 4
 *          rising or falling successions.
 * https://oeis.org/A001268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [t^4] S[n](t)/2, where (A002464/A001100) S[n](t) =
 *            (n+1-t)*S[n-1](t) - (1-t)*(n-2+3t)*S[n-2](t) -
 *            (1-t)^2*(n-5+t)*S[n-3](t) + (1-t)^3*(n-3)*S[n-4](t), with S[2]/2
 *            = t, S[3]/2 = 2t+t^2, S[4]/2 = 1+5t+5t^2+t^3, S[5]/2 =
 *            7+20t+24t^2+8t^3+t^4 (only coefficients up to t^4 are kept)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   a1268_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a1268_add.h"

static seq_t a1268_h[4][5];
static idx_t a1268_cur = 0;

static void a1268_init(void) {
  static const int init[4][5] = {
    {0, 1, 0, 0, 0},
    {0, 2, 1, 0, 0},
    {1, 5, 5, 1, 0},
    {7, 20, 24, 8, 1}
  };
  int i, k;
  for (i = 0; i < 4; i++)
    for (k = 0; k < 5; k++)
      a1268_h[i][k] = (seq_t)init[i][k];
  a1268_cur = 5;
}

static void a1268_step(void) {
  long long nn = (long long)(a1268_cur + 1);
  long long A[4], B[4], C[4], D[4];
  seq_t nw[5];
  int k, d, i;
  A[0] = nn + 1; A[1] = -1; A[2] = 0; A[3] = 0;
  B[0] = -(nn - 2); B[1] = nn - 5; B[2] = 3; B[3] = 0;
  C[0] = -(nn - 5); C[1] = 2 * nn - 11; C[2] = 7 - nn; C[3] = -1;
  D[0] = nn - 3; D[1] = -3 * (nn - 3); D[2] = 3 * (nn - 3); D[3] = -(nn - 3);
  for (k = 0; k < 5; k++) {
    seq_t pos = 0, neg = 0;
    for (d = 0; d < 4 && d <= k; d++) {
      a1268_add(&pos, &neg, A[d], &a1268_h[3][k - d]);
      a1268_add(&pos, &neg, B[d], &a1268_h[2][k - d]);
      a1268_add(&pos, &neg, C[d], &a1268_h[1][k - d]);
      a1268_add(&pos, &neg, D[d], &a1268_h[0][k - d]);
    }
    nw[k] = pos - neg;
  }
  for (i = 0; i < 3; i++)
    for (k = 0; k < 5; k++)
      a1268_h[i][k] = a1268_h[i + 1][k];
  for (k = 0; k < 5; k++)
    a1268_h[3][k] = nw[k];
  a1268_cur++;
}

seq_t a(idx_t n) {
  if (n < 5) return 0;
  if (a1268_cur < 5 || n < a1268_cur) a1268_init();
  while (a1268_cur < n) a1268_step();
  return a1268_h[3][4];
}
