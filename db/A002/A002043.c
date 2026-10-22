/*
 * A002043  MacMahon's solid partitions of n in which 2 is the smallest
 *          summand.
 * https://oeis.org/A002043
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(p(n-2)-p(n-3)-p(n-4)+p(n-5))+p(n-6)-p(n-7), where p(n)
 *            = A000294(n), g.f. Product_{k>=1} 1/(1-x^k)^(k*(k+1)/2), and
 *            p(n) = 0 for n < 0. Exact (big) build: Euler transform m*p(m) =
 *            Sum_{k=1..m} c(k)*p(m-k), with c(k) = Sum_{d|k} d*d*(d+1)/2.
 *            Fixed-width build: the same product expanded w...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 34, u64 n <= 78, u128 n <= 186,
 *            big any n
 * Helpers:   a2043_c, a2043_from_u128, a2043_inv, a2043_is_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a2043_c.h"
#include "../utils/a2043_from_u128.h"
#include "../utils/a2043_inv.h"
#include "../utils/a2043_is_fixed.h"

#define A2043_PMAX 32768

static seq_t a2043_bp[A2043_PMAX];
static seq_t a2043_bc[A2043_PMAX];
static idx_t a2043_blen = 0;

static unsigned __int128 *a2043_fp = NULL;
static idx_t a2043_flen = 0;

static int a2043_mode = -1;

static void a2043_big_extend(idx_t N) {
  if (a2043_blen == 0) {
    a2043_bp[0] = 1;
    a2043_bc[0] = 0;
    a2043_blen = 1;
  }
  while (a2043_blen <= N) {
    idx_t m = a2043_blen, k;
    seq_t s = 0;
    a2043_bc[m] = (seq_t)a2043_c(m);
    for (k = 1; k <= m; k++) s += a2043_bc[k] * a2043_bp[m - k];
    a2043_bp[m] = s / (seq_t)m;
    a2043_blen++;
  }
}

static void a2043_fixed_build(idx_t N) {
  idx_t T, k, j, m, i;
  unsigned __int128 *f, *coef;
  if (N < a2043_flen) return;
  T = N + 16;
  f = (unsigned __int128 *)malloc((T + 1) * sizeof(unsigned __int128));
  coef = (unsigned __int128 *)malloc((T + 1) * sizeof(unsigned __int128));
  for (i = 0; i <= T; i++) f[i] = 0;
  f[0] = 1;
  for (k = 1; k <= T; k++) {
    uint64_t b = (uint64_t)k * (k + 1) / 2;
    idx_t J = T / k;
    unsigned __int128 Nn = 1, Dd = 1;
    long long tw = 0;
    coef[0] = 1;
    for (j = 1; j <= J; j++) {
      uint64_t x = b + j - 1, y = j;
      while ((x & 1) == 0) { x >>= 1; tw++; }
      while ((y & 1) == 0) { y >>= 1; tw--; }
      Nn *= x;
      Dd *= y;
      if (tw >= 128) coef[j] = 0;
      else coef[j] = (Nn * a2043_inv(Dd)) << tw;
    }
    for (m = T; m >= k; m--) {
      unsigned __int128 s = f[m];
      for (j = 1; j * k <= m; j++) s += coef[j] * f[m - j * k];
      f[m] = s;
    }
  }
  free(coef);
  if (a2043_fp) free(a2043_fp);
  a2043_fp = f;
  a2043_flen = T + 1;
}

seq_t a(idx_t n) {
  if (n < 2) return 0;
  if (a2043_mode < 0) a2043_mode = a2043_is_fixed();
  if (a2043_mode) {
    unsigned __int128 p[8];
    unsigned __int128 r;
    int i;
    a2043_fixed_build(n);
    for (i = 2; i <= 7; i++) p[i] = (n >= (idx_t)i) ? a2043_fp[n - i] : 0;
    r = 3 * (p[2] - p[3] - p[4] + p[5]) + p[6] - p[7];
    return a2043_from_u128(r);
  } else {
    seq_t p[8];
    int i;
    if (n - 2 >= A2043_PMAX) return 0;
    a2043_big_extend(n - 2);
    for (i = 2; i <= 7; i++) {
      if (n >= (idx_t)i) p[i] = a2043_bp[n - i];
      else p[i] = 0;
    }
    return (3 * (p[2] + p[5]) + p[6]) - (3 * (p[3] + p[4]) + p[7]);
  }
}
