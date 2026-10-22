/*
 * A001282  One-half of the number of permutations p of [n] having exactly
 *          three runs of adjacent entries differing by 1.
 * https://oeis.org/A001282
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   from the OEIS g.f., writing the polynomial in k in the basis
 *            (k+1)(k+2)...(k+m) gives 3*a(n) = [x^n] Sum_{m=1..6} A_m(x) *
 *            G_m(x), with G_m = Sum_{j>=m} j!*y^j, y = x(1-x)/(1+x), A_1 =
 *            -3x^5/((1+x)^3(1-x)^4), A_2 = 3x^4(x-3)/((1+x)^3(1-x)^5), A_3 =
 *            3(8x+4)x^3/((1+x)^3(1-x)^6), A_4 = 3(10-2x)x^2/(...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   a677_from_u128, seq_big_mode
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/a677_from_u128.h"
#include "../utils/seq_big_mode.h"

#define PD 32

typedef __int128 i128_t;
typedef unsigned __int128 u128_t;

static void p_mulf(long long *p, long long c0, long long c1, int times) {
  int t, i;
  for (t = 0; t < times; t++) {
    long long r[PD];
    memset(r, 0, sizeof(r));
    for (i = 0; i < PD; i++) {
      if (p[i] == 0) continue;
      r[i] += c0 * p[i];
      if (i + 1 < PD) r[i + 1] += c1 * p[i];
    }
    memcpy(p, r, sizeof(r));
  }
}

static void num_m(int m, long long *p) {
  memset(p, 0, sizeof(long long) * PD);
  switch (m) {
    case 1: p[0] = -3; p_mulf(p, 0, 1, 5); p_mulf(p, 1, -1, 5); break;
    case 2: p[0] = 3; p_mulf(p, 0, 1, 4); p_mulf(p, -3, 1, 1); p_mulf(p, 1, -1, 4); break;
    case 3: p[0] = 3; p_mulf(p, 4, 8, 1); p_mulf(p, 0, 1, 3); p_mulf(p, 1, -1, 3); break;
    case 4: p[0] = 3; p_mulf(p, 10, -2, 1); p_mulf(p, 1, 1, 1); p_mulf(p, 0, 1, 2); p_mulf(p, 1, -1, 2); break;
    case 5: p[0] = -18; p_mulf(p, 0, 1, 1); p_mulf(p, 1, 1, 2); p_mulf(p, 1, -1, 1); break;
    default: p[0] = 2; p_mulf(p, 1, 1, 3); break;
  }
}

static void series(i128_t *s, const long long *num, int cpow, idx_t n) {
  idx_t i;
  int t;
  for (i = 0; i <= n; i++) s[i] = (i < PD) ? (i128_t)num[i] : 0;
  for (t = 0; t < cpow; t++)
    for (i = 1; i <= n; i++) s[i] -= s[i - 1];
  for (t = 0; t < 9; t++)
    for (i = 1; i <= n; i++) s[i] += s[i - 1];
}

seq_t a(idx_t n) {
  static const int base[5] = {1, 1, 0, 0, 2};
  long long NQ[PD], NR[PD], p[PD], t[PD];
  int m, j, i;
  idx_t k;
  i128_t *q, *rs, rn;

  if (n < 6) return 0;

  memset(NQ, 0, sizeof(NQ));
  memset(NR, 0, sizeof(NR));
  for (m = 1; m <= 6; m++) {
    long long f = 1;
    num_m(m, p);
    for (i = 0; i < PD; i++) NQ[i] += p[i];
    for (j = 0; j < m; j++) {
      if (j > 0) f *= j;
      memcpy(t, p, sizeof(t));
      p_mulf(t, 0, 1, j);
      p_mulf(t, 1, -1, j);
      p_mulf(t, 1, 1, 5 - j);
      for (i = 0; i < PD; i++) NR[i] += f * t[i];
    }
  }

  q = (i128_t *)malloc((size_t)(n + 1) * sizeof(i128_t));
  rs = (i128_t *)malloc((size_t)(n + 1) * sizeof(i128_t));
  series(q, NQ, 3, n);
  series(rs, NR, 8, n);
  rn = rs[n];
  free(rs);

  if (!seq_big_mode()) {
    const u128_t INV3 = ((u128_t)0xAAAAAAAAAAAAAAAAULL << 64) | (u128_t)0xAAAAAAAAAAAAAAABULL;
    u128_t h1 = 0, h2 = 0, h3 = 0, h4 = 0, acc = 0, hj;
    for (k = 0; k <= n; k++) {
      if (k < 5) hj = (u128_t)base[k];
      else hj = (u128_t)(k + 1) * h1 - (u128_t)(k - 2) * h2
              - (u128_t)(k - 5) * h3 + (u128_t)(k - 3) * h4;
      acc += (u128_t)q[n - k] * hj;
      h4 = h3; h3 = h2; h2 = h1; h1 = hj;
    }
    acc -= (u128_t)rn;
    acc *= INV3;
    free(q);
    return a677_from_u128(acc);
  } else {
    seq_t h1 = 0, h2 = 0, h3 = 0, h4 = 0, hj = 0, pos = 0, neg = 0;
    for (k = 0; k <= n; k++) {
      i128_t w = q[n - k];
      if (k < 5) {
        hj = (seq_t)base[k];
      } else {
        seq_t pl = (seq_t)(k + 1) * h1 + (seq_t)(k - 3) * h4;
        seq_t mi = (seq_t)(k - 2) * h2 + (seq_t)(k - 5) * h3;
        hj = pl - mi;
      }
      if (w > 0) pos += a677_from_u128((u128_t)w) * hj;
      else if (w < 0) neg += a677_from_u128((u128_t)(-w)) * hj;
      h4 = h3; h3 = h2; h2 = h1; h1 = hj;
    }
    if (rn > 0) neg += a677_from_u128((u128_t)rn);
    else if (rn < 0) pos += a677_from_u128((u128_t)(-rn));
    free(q);
    return (pos - neg) / 3;
  }
}
