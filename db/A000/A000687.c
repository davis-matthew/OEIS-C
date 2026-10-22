/*
 * A000687  Boustrophedon transform (first version) of Fibonacci numbers
 *          0,1,1,2,3,5,...
 * https://oeis.org/A000687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Boustrophedon transform of b = 1, 0, 1, 1, 2, 3, 5, ... (b(0) =
 *            1, b(k) = F(k-1) for k >= 1): T(k,0) = b(k), T(k,j) = T(k,j-1) +
 *            T(k-1,k-j) for 1 <= j <= k, a(n) = T(n,n)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   grow_buf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/grow_buf.h"

static seq_t *R = NULL, *P = NULL, *Q = NULL;
static idx_t done = 0, cap = 0;
static seq_t fx, fy;

seq_t a(idx_t n) {
  if (n >= cap) {
    idx_t nc = cap ? cap * 2 : 64;
    while (nc <= n) nc *= 2;
    R = grow_buf(R, done, nc);
    P = grow_buf(P, done, nc);
    Q = grow_buf(Q, done, nc);
    cap = nc;
  }
  if (done == 0) {
    R[0] = 1;
    P[0] = 1;
    fx = 0; /* F(0), input for row 1 */
    fy = 1; /* F(1) */
    done = 1;
  }
  while (done <= n) {
    idx_t k = done, j;
    seq_t *t;
    seq_t nf;
    Q[0] = fx;
    for (j = 1; j <= k; j++) Q[j] = Q[j - 1] + P[k - j];
    R[k] = Q[k];
    t = P; P = Q; Q = t;
    nf = fx + fy;
    fx = fy;
    fy = nf;
    done++;
  }
  return R[n];
}
