/*
 * A000744  Boustrophedon transform (second version) of Fibonacci numbers
 *          1,1,2,3,...
 * https://oeis.org/A000744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n,n) of the boustrophedon triangle T(n,0) =
 *            A000045(n+1), T(n,k) = T(n,k-1) + T(n-1,n-k) (equivalently a(n)
 *            = Sum_{k=0..n} A109449(n,k)*A000045(k+1)); F(1) = 1, F(2) = 1,
 *            F(m) = F(m-1) + F(m-2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   a306_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a306_grow.h"

static seq_t *prev_row = 0;
static seq_t *cur_row = 0;
static seq_t *vals = 0;
static idx_t cap = 0;
static idx_t done = 0;
static seq_t fa, fb;

static void ensure_cap(idx_t need) {
  if (need <= cap) return;
  idx_t nc = cap ? cap : 16;
  while (nc < need) nc *= 2;
  prev_row = a306_grow(prev_row, cap, nc);
  cur_row = a306_grow(cur_row, cap, nc);
  vals = a306_grow(vals, cap, nc);
  cap = nc;
}

seq_t a(idx_t n) {
  if (done == 0) {
    fa = 1;
    fb = 1;
  }
  ensure_cap(n + 1);
  while (done <= n) {
    idx_t m = done;
    cur_row[0] = fa;
    for (idx_t k = 1; k <= m; k++)
      cur_row[k] = cur_row[k - 1] + prev_row[m - k];
    vals[m] = cur_row[m];
    seq_t *t = prev_row;
    prev_row = cur_row;
    cur_row = t;
    seq_t f = fa + fb;
    fa = fb;
    fb = f;
    done++;
  }
  return vals[n];
}
