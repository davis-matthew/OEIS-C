/*
 * A003610  Symmetries in unrooted (1,3) trees on 2n vertices.
 * https://oeis.org/A003610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{T} |Aut(T)| over unrooted (1,3)-trees T on 2n
 *            vertices; a(1) = 2. Count by internal nodes. Let P_m(x) = Sum
 *            over planted binary trees of |Aut|^m x^size. Then P_m(x) = 1 +
 *            x*(P_m(x)^2 + (2^(m+1)-1)*P_{2m}(x^2))/2. Let N = n-1 and Q_m =
 *            P_m truncated to degree <= (N-1)/2. Then a(n) = [x...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   a3610_find, a3610_isbig, a3610_list
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/a3610_find.h"
#include "../utils/a3610_isbig.h"
#include "../utils/a3610_list.h"

#define A3610_MAXM 200

/* ---------------- arbitrary precision path (exact) ---------------- */
static idx_t b_cap = 0;
static int b_nm = 0;
static idx_t b_mv[A3610_MAXM], b_D[A3610_MAXM];
static seq_t *b_P[A3610_MAXM];

static void a3610_big_build(idx_t N) {
  int i;
  for (i = 0; i < b_nm; i++) free(b_P[i]);
  b_nm = a3610_list(2 * N + 2, b_mv);
  for (i = 0; i < b_nm; i++) {
    b_D[i] = (b_mv[i] == 1) ? N / 2 : N / b_mv[i];
    b_P[i] = (seq_t *)calloc((size_t)(b_D[i] + 1), sizeof(seq_t));
  }
  for (i = b_nm - 1; i >= 0; i--) {
    idx_t m = b_mv[i], D = b_D[i], k, e;
    seq_t *P = b_P[i];
    seq_t c = 1;
    seq_t *P2 = NULL;
    int j;
    P[0] = 1;
    if (D == 0) continue;
    j = a3610_find(b_mv, b_nm, 2 * m);
    if (j >= 0) P2 = b_P[j];
    for (e = 0; e < m + 1; e++) c += c;
    c -= 1;
    for (k = 1; k <= D; k++) {
      idx_t t = k - 1, ii;
      seq_t s = 0;
      for (ii = 0; 2 * ii < t; ii++) s += P[ii] * P[t - ii];
      if (t % 2 == 0) {
        idx_t hh = t / 2;
        seq_t q = P[hh] * P[hh];
        if (P2) q += c * P2[hh];
        else if (hh == 0) q += c;
        s += q / 2;
      }
      P[k] = s;
    }
  }
  b_cap = N;
}

static seq_t a3610_big(idx_t N) {
  seq_t *P1, *P2, *P3;
  idx_t h = (N - 1) / 2, T = N - 1, l, k;
  seq_t cube = 0, mix = 0, trip = 0, res;
  if (N > b_cap) {
    idx_t nc = b_cap + b_cap / 2;
    a3610_big_build(N > nc ? N : nc);
  }
  P1 = b_P[a3610_find(b_mv, b_nm, 1)];
  P2 = b_P[a3610_find(b_mv, b_nm, 2)];
  P3 = b_P[a3610_find(b_mv, b_nm, 3)];
  for (l = 0; l <= h; l++) {
    idx_t t = T - l, lo, hi, i;
    seq_t S = 0;
    if (t > 2 * h) continue;
    lo = t > h ? t - h : 0;
    hi = t < h ? t : h;
    for (i = lo; i <= hi; i++) S += P1[i] * P1[t - i];
    cube += P1[l] * S;
  }
  for (k = 0; 2 * k <= T && k <= h; k++) {
    idx_t j = T - 2 * k;
    if (j <= h) mix += P2[k] * P1[j];
  }
  if (T % 3 == 0) trip = P3[T / 3];
  res = (cube + 9 * mix + 26 * trip) / 6;
  if (N % 2 == 0) {
    idx_t hN = N / 2;
    res += (P1[hN] * P1[hN] + 3 * P2[hN]) / 2;
  }
  return res;
}

/* ---------------- fixed width path: arithmetic mod 2^256 ---------------- */
typedef struct { uint32_t l[8]; } a3610_u;

static a3610_u u_set(uint32_t v) { a3610_u r; memset(&r, 0, sizeof r); r.l[0] = v; return r; }
static a3610_u u_add(a3610_u x, a3610_u y) {
  a3610_u r; uint64_t c = 0; int i;
  for (i = 0; i < 8; i++) { c += (uint64_t)x.l[i] + y.l[i]; r.l[i] = (uint32_t)c; c >>= 32; }
  return r;
}
static a3610_u u_mul(a3610_u x, a3610_u y) {
  a3610_u r; int i, j;
  memset(&r, 0, sizeof r);
  for (i = 0; i < 8; i++) {
    uint64_t c = 0;
    for (j = 0; i + j < 8; j++) {
      uint64_t cur = (uint64_t)x.l[i] * y.l[j] + r.l[i + j] + c;
      r.l[i + j] = (uint32_t)cur;
      c = cur >> 32;
    }
  }
  return r;
}
static a3610_u u_shr1(a3610_u x) {
  a3610_u r; int i;
  for (i = 0; i < 8; i++) r.l[i] = (x.l[i] >> 1) | (i < 7 ? (uint32_t)(x.l[i + 1] << 31) : 0u);
  return r;
}

static seq_t a3610_fixed(idx_t N) {
  idx_t mv[A3610_MAXM], D[A3610_MAXM];
  a3610_u *P[A3610_MAXM];
  a3610_u *P1, *P2, *P3, cube, mix, trip, res, inv3, neg1;
  idx_t h = (N - 1) / 2, T = N - 1, l, k;
  int nm, i;
  seq_t out, B;
  nm = a3610_list(2 * N + 2, mv);
  for (i = 0; i < 8; i++) { inv3.l[i] = 0xAAAAAAAAu; neg1.l[i] = 0xFFFFFFFFu; }
  inv3.l[0] = 0xAAAAAAABu;
  for (i = 0; i < nm; i++) {
    D[i] = (mv[i] == 1) ? N / 2 : N / mv[i];
    P[i] = (a3610_u *)malloc((size_t)(D[i] + 1) * sizeof(a3610_u));
  }
  for (i = nm - 1; i >= 0; i--) {
    idx_t m = mv[i], Dm = D[i];
    a3610_u *Pm = P[i], *Q = NULL, c;
    int j;
    Pm[0] = u_set(1);
    if (Dm == 0) continue;
    j = a3610_find(mv, nm, 2 * m);
    if (j >= 0) Q = P[j];
    c = u_set(0);
    if (m + 1 < 256) c.l[(m + 1) / 32] = 1u << ((m + 1) % 32);
    c = u_add(c, neg1);
    for (k = 1; k <= Dm; k++) {
      idx_t t = k - 1, ii;
      a3610_u s = u_set(0);
      for (ii = 0; 2 * ii < t; ii++) s = u_add(s, u_mul(Pm[ii], Pm[t - ii]));
      if (t % 2 == 0) {
        idx_t hh = t / 2;
        a3610_u q = u_mul(Pm[hh], Pm[hh]);
        if (Q) q = u_add(q, u_mul(c, Q[hh]));
        else if (hh == 0) q = u_add(q, c);
        s = u_add(s, u_shr1(q));
      }
      Pm[k] = s;
    }
  }
  P1 = P[a3610_find(mv, nm, 1)];
  P2 = P[a3610_find(mv, nm, 2)];
  P3 = P[a3610_find(mv, nm, 3)];
  cube = u_set(0); mix = u_set(0); trip = u_set(0);
  for (l = 0; l <= h; l++) {
    idx_t t = T - l, lo, hi, ii;
    a3610_u S = u_set(0);
    if (t > 2 * h) continue;
    lo = t > h ? t - h : 0;
    hi = t < h ? t : h;
    for (ii = lo; ii <= hi; ii++) S = u_add(S, u_mul(P1[ii], P1[t - ii]));
    cube = u_add(cube, u_mul(P1[l], S));
  }
  for (k = 0; 2 * k <= T && k <= h; k++) {
    idx_t j = T - 2 * k;
    if (j <= h) mix = u_add(mix, u_mul(P2[k], P1[j]));
  }
  if (T % 3 == 0) trip = P3[T / 3];
  res = u_add(cube, u_add(u_mul(u_set(9), mix), u_mul(u_set(26), trip)));
  res = u_shr1(u_mul(res, inv3));
  if (N % 2 == 0) {
    idx_t hN = N / 2;
    a3610_u e = u_add(u_mul(P1[hN], P1[hN]), u_mul(u_set(3), P2[hN]));
    res = u_add(res, u_shr1(e));
  }
  for (i = 0; i < nm; i++) free(P[i]);
  B = (seq_t)65536;
  B = B * (seq_t)65536;
  out = 0;
  for (i = 7; i >= 0; i--) out = out * B + (seq_t)res.l[i];
  return out;
}

seq_t a(idx_t n) {
  idx_t N;
  if (n <= 1) return 2;
  N = n - 1;
  if (a3610_isbig()) return a3610_big(N);
  if (n > 400) return 0; /* far beyond any fixed width */
  return a3610_fixed(N);
}
