/*
 * A003164  Numerators of Van der Pol numbers.
 * https://oeis.org/A003164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, nice
 * Formula:   E.g.f. for the fractions: x^3/(6*x*(exp(x)+1)-12*(exp(x)-1)),
 *            whose denominator is Sum_{k>=3} 6*(k-2)*x^k/k!. The fractions
 *            are built from the recurrence r(0) = 1, r(m) = -Sum_{j=1..m}
 *            binomial(m,j)*6/((j+2)*(j+3))*r(m-j), using exact reduced
 *            rationals; a(n) = numerator(r(n)). Fixed-width builds...
 * Range:     i8 n <= 9, i16 n <= 15, i32 n <= 19, i64 n <= 31, i128 n <= 49,
 *            big any n
 * Helpers:   A_grow, from_i128, seq_is_big, vdp_gcd, vdp_gcd128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A_grow.h"
#include "../utils/seq_is_big.h"
#include "../utils/from_i128.h"
#include "../utils/vdp_gcd.h"
#include "../utils/vdp_gcd128.h"

static seq_t *vdp_num = NULL;
static seq_t *vdp_den = NULL;
static idx_t vdp_cap = 0;
static idx_t vdp_len = 0;

static seq_t vdp_big(idx_t n) {
  if (n >= vdp_cap) {
    idx_t nc = vdp_cap * 2;
    if (nc < n + 1) nc = n + 1;
    if (nc < 16) nc = 16;
    vdp_num = A_grow(vdp_num, vdp_len, nc);
    vdp_den = A_grow(vdp_den, vdp_len, nc);
    vdp_cap = nc;
  }
  if (vdp_len == 0) {
    vdp_num[0] = 1;
    vdp_den[0] = 1;
    vdp_len = 1;
  }
  while (vdp_len <= n) {
    idx_t m = vdp_len;
    seq_t P = 0, Q = 1;
    seq_t C = 1;
    idx_t j;
    for (j = 1; j <= m; j++) {
      seq_t cn, cd, g, N, D, g1, g2, an, bd, t, g3;
      C = C * (seq_t)(m - j + 1) / (seq_t)j;
      N = vdp_num[m - j];
      if (N == 0) continue;
      D = vdp_den[m - j];
      cn = C * 6;
      cd = (seq_t)(j + 2) * (seq_t)(j + 3);
      g = vdp_gcd(cn, cd);
      cn /= g;
      cd /= g;
      g1 = vdp_gcd(cn, D);
      g2 = vdp_gcd(N, cd);
      an = (cn / g1) * (N / g2);
      bd = (cd / g2) * (D / g1);
      g = vdp_gcd(Q, bd);
      t = P * (bd / g) + an * (Q / g);
      if (t == 0) {
        P = 0;
        Q = 1;
      } else {
        g3 = vdp_gcd(t, g);
        P = t / g3;
        Q = (Q / g) * (bd / g3);
      }
    }
    vdp_num[m] = -P;
    vdp_den[m] = Q;
    vdp_len++;
  }
  return vdp_num[n];
}

static __int128 *vf_num = NULL;
static __int128 *vf_den = NULL;
static idx_t vf_cap = 0;
static idx_t vf_len = 0;

static __int128 vdp_fixed(idx_t n) {
  if (n >= vf_cap) {
    idx_t nc = vf_cap * 2;
    if (nc < n + 1) nc = n + 1;
    if (nc < 16) nc = 16;
    vf_num = (__int128 *)realloc(vf_num, (size_t)nc * sizeof(__int128));
    vf_den = (__int128 *)realloc(vf_den, (size_t)nc * sizeof(__int128));
    vf_cap = nc;
  }
  if (vf_len == 0) {
    vf_num[0] = 1;
    vf_den[0] = 1;
    vf_len = 1;
  }
  while (vf_len <= n) {
    idx_t m = vf_len;
    __int128 P = 0, Q = 1;
    __int128 C = 1;
    idx_t j;
    for (j = 1; j <= m; j++) {
      __int128 cn, cd, g, N, D, g1, g2, an, bd, t, g3, gc;
      /* C = binomial(m, j), kept exact */
      gc = vdp_gcd128(C, (__int128)j);
      C = (C / gc) * ((__int128)(m - j + 1) / ((__int128)j / gc));
      N = vf_num[m - j];
      if (N == 0) continue;
      D = vf_den[m - j];
      cn = C * 6;
      cd = (__int128)(j + 2) * (__int128)(j + 3);
      g = vdp_gcd128(cn, cd);
      cn /= g;
      cd /= g;
      g1 = vdp_gcd128(cn, D);
      g2 = vdp_gcd128(N, cd);
      an = (cn / g1) * (N / g2);
      bd = (cd / g2) * (D / g1);
      g = vdp_gcd128(Q, bd);
      t = P * (bd / g) + an * (Q / g);
      if (t == 0) {
        P = 0;
        Q = 1;
      } else {
        g3 = vdp_gcd128(t, g);
        P = t / g3;
        Q = (Q / g) * (bd / g3);
      }
    }
    vf_num[m] = -P;
    vf_den[m] = Q;
    vf_len++;
  }
  return vf_num[n];
}

seq_t a(idx_t n) {
  if (seq_is_big()) return vdp_big(n);
  return from_i128(vdp_fixed(n));
}
