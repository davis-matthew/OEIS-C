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
 * Validated: standard, pass, 44 terms through n = 200 (2026-11-03)
 * Helpers:   gcd_i128, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/gcd_i128.h"
#include "../utils/mem.h"

static seq_t *A_grow(seq_t *old, idx_t oldlen, idx_t newlen)
{
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    idx_t i;
    for (i = 0; i < newlen; i++) p[i] = 0;
    for (i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) mem_free(old);
    return p;
}

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

static seq_t from_i128(__int128 v)
{
    if (v >= -(__int128)4000000000000000000LL && v <= (__int128)4000000000000000000LL)
        return (seq_t)(long long)v;
    {
        __int128 q = v / ((__int128)1 << 32);
        long long rem = (long long)(v - q * ((__int128)1 << 32));
        return from_i128(q) * (seq_t)4294967296LL + (seq_t)rem;
    }
}

static seq_t vdp_gcd(seq_t x, seq_t y) {
  if (x < 0) x = -x;
  if (y < 0) y = -y;
  while (y != 0) {
    seq_t t = x % y;
    x = y;
    y = t;
  }
  return x;
}

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
      gc = gcd_i128(C, (__int128)j);
      C = (C / gc) * ((__int128)(m - j + 1) / ((__int128)j / gc));
      N = vf_num[m - j];
      if (N == 0) continue;
      D = vf_den[m - j];
      cn = C * 6;
      cd = (__int128)(j + 2) * (__int128)(j + 3);
      g = gcd_i128(cn, cd);
      cn /= g;
      cd /= g;
      g1 = gcd_i128(cn, D);
      g2 = gcd_i128(N, cd);
      an = (cn / g1) * (N / g2);
      bd = (cd / g2) * (D / g1);
      g = gcd_i128(Q, bd);
      t = P * (bd / g) + an * (Q / g);
      if (t == 0) {
        P = 0;
        Q = 1;
      } else {
        g3 = gcd_i128(t, g);
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
