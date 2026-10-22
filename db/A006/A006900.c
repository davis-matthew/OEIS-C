/*
 * A006900  Log of g.f. for rooted trees.
 * https://oeis.org/A006900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = k(n+1), where k(N) = N! [x^N] log(1 + Sum_{m>=1}
 *            A000081(m) x^m/m!), computed by the recurrence k(N) = t(N) -
 *            Sum_{j=1..N-1} binomial(N-1,j-1)*k(j)*t(N-j), with t = A000081
 *            from t(1) = 1, t(m) = (1/(m-1)) Sum_{j=1..m-1} s(j)*t(m-j), s(j)
 *            = Sum_{d|j} d*t(d)
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   alloc_zero, from_i128, grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/from_i128.h"
#include "../utils/seq_is_fixed_width.h"

/* Fixed-width builds: work modulo 2^128 (only + - * after the exact t's),
   so any result that fits in seq_t comes out exact. */
static seq_t a_fixed(idx_t n) {
  enum { M = 100 };
  unsigned __int128 t[M + 2], s[M + 2], k[M + 2], row[M + 2];
  idx_t N = n + 1, m, j, d;
  if (N > M) N = M;
  for (m = 1; m <= N; m++) {
    if (m == 1) {
      t[1] = 1;
    } else {
      unsigned __int128 acc = 0;
      for (j = 1; j < m; j++) acc += s[j] * t[m - j];
      t[m] = acc / (unsigned __int128)(m - 1);
    }
    s[m] = 0;
    for (d = 1; d <= m; d++)
      if (m % d == 0) s[m] += (unsigned __int128)d * t[d];
    if (m == 1) {
      row[0] = 1;
    } else {
      row[m - 1] = 0;
      for (j = m - 1; j >= 1; j--) row[j] += row[j - 1];
    }
    unsigned __int128 kk = t[m];
    for (j = 1; j < m; j++) kk -= row[j - 1] * k[j] * t[m - j];
    k[m] = kk;
  }
  return from_i128((__int128)k[N]);
}

static seq_t *gt, *gs, *gk, *gr;
static idx_t gcap = 0, gn = 0;

static void big_extend(idx_t N) {
  idx_t m, j, d;
  if (N + 1 > gcap) {
    idx_t nc = gcap ? gcap : 16;
    while (nc < N + 1) nc *= 2;
    if (gcap == 0) {
      gt = alloc_zero(nc);
      gs = alloc_zero(nc);
      gk = alloc_zero(nc);
      gr = alloc_zero(nc);
    } else {
      gt = grow_zero(gt, gcap, nc);
      gs = grow_zero(gs, gcap, nc);
      gk = grow_zero(gk, gcap, nc);
      gr = grow_zero(gr, gcap, nc);
    }
    gcap = nc;
  }
  for (m = gn + 1; m <= N; m++) {
    if (m == 1) {
      gt[1] = 1;
    } else {
      seq_t acc = 0;
      for (j = 1; j < m; j++) acc += gs[j] * gt[m - j];
      gt[m] = acc / (seq_t)(m - 1);
    }
    seq_t sm = 0;
    for (d = 1; d * d <= m; d++) {
      if (m % d == 0) {
        idx_t e = m / d;
        sm += (seq_t)d * gt[d];
        if (e != d) sm += (seq_t)e * gt[e];
      }
    }
    gs[m] = sm;
    if (m == 1) {
      gr[0] = 1;
    } else {
      gr[m - 1] = 0;
      for (j = m - 1; j >= 1; j--) gr[j] += gr[j - 1];
    }
    seq_t kk = gt[m];
    for (j = 1; j < m; j++) kk -= gr[j - 1] * gk[j] * gt[m - j];
    gk[m] = kk;
  }
  if (N > gn) gn = N;
}

seq_t a(idx_t n) {
  if (seq_is_fixed_width()) return a_fixed(n);
  if (n + 1 > gn) big_extend(n + 1);
  return gk[n + 1];
}
