/*
 * A003123  Number of Hamiltonian rooted triangulations with n internal nodes
 *          and 4 external nodes.
 * https://oeis.org/A003123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(n,4) = (1/m) * Sum_{j=0..n} (j+4)*e(j)*C(j+2)*[t^(n-j)]
 *            F(t)^(-m), where m = n+4, C(i) = Catalan numbers, F(t) =
 *            Sum_{i>=0} C(i)^2 t^i, e(j) = [y^j] C(y)^4 =
 *            4/(2j+4)*binomial(2j+4,j). (The Hamiltonian cycle splits the map
 *            into a triangulated (n+4)-gon and an (n+4)-gon dissected into
 *            the...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   a3123_big, a3123_is_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/a3123_big.h"
#include "../utils/a3123_is_fixed.h"

typedef unsigned __int128 a3123_u128;

static seq_t a3123_from(a3123_u128 v) {
  seq_t r = (seq_t)(uint64_t)(v >> 64);
  int i;
  for (i = 0; i < 8; i++) r = r * 256;
  r = r + (seq_t)(uint64_t)v;
  return r;
}

static void a3123_smul(const a3123_u128 *x, const a3123_u128 *y, a3123_u128 *out, idx_t L) {
  idx_t i, j;
  for (i = 0; i < L; i++) {
    a3123_u128 s = 0;
    for (j = 0; j <= i; j++) s += x[j] * y[i - j];
    out[i] = s;
  }
}

/* exact modulo 2^128, ring operations only */
static seq_t a3123_fixed(idx_t n) {
  idx_t m = n + 4, L = n + 1, NC = n + 3, i, j, e;
  size_t sz = sizeof(a3123_u128);
  a3123_u128 *cat = (a3123_u128 *)calloc(NC, sz);
  a3123_u128 *c = (a3123_u128 *)calloc(L, sz);
  a3123_u128 *c2 = (a3123_u128 *)calloc(L, sz);
  a3123_u128 *c4 = (a3123_u128 *)calloc(L, sz);
  a3123_u128 *g = (a3123_u128 *)calloc(L, sz);
  a3123_u128 *G = (a3123_u128 *)calloc(L, sz);
  a3123_u128 *B = (a3123_u128 *)calloc(L, sz);
  a3123_u128 *T = (a3123_u128 *)calloc(L, sz);
  a3123_u128 res = 0;

  cat[0] = 1;
  for (i = 1; i < NC; i++) {
    a3123_u128 s = 0;
    for (j = 0; j < i; j++) s += cat[j] * cat[i - 1 - j];
    cat[i] = s;
  }
  for (i = 0; i < L; i++) c[i] = cat[i] * cat[i];
  a3123_smul(cat, cat, c2, L);
  a3123_smul(c2, c2, c4, L);

  g[0] = 1;
  for (j = 1; j < L; j++) {
    a3123_u128 s = 0;
    for (i = 1; i <= j; i++) s += c[i] * g[j - i];
    g[j] = (a3123_u128)0 - s;
  }

  G[0] = 1;
  memcpy(B, g, L * sz);
  e = m;
  while (e) {
    if (e & 1) { a3123_smul(G, B, T, L); memcpy(G, T, L * sz); }
    e >>= 1;
    if (e) { a3123_smul(B, B, T, L); memcpy(B, T, L * sz); }
  }

  a3123_smul(G, g, T, L); /* T = G^(m+1) */
  for (j = 0; j < L; j++) {
    a3123_u128 s = G[j];
    for (i = 1; i <= j; i++) s += (a3123_u128)i * c[i] * T[j - i];
    B[j] = s; /* Q = G^m + t F' G^(m+1) */
  }

  for (j = 0; j <= n; j++) res += c4[j] * cat[j + 2] * B[n - j];

  free(cat); free(c); free(c2); free(c4); free(g); free(G); free(B); free(T);
  return a3123_from(res);
}

seq_t a(idx_t n) {
  if (a3123_is_fixed()) return a3123_fixed(n);
  return a3123_big(n);
}
