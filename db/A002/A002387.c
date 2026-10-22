/*
 * A002387  Least k such that H(k) > n, where H(k) is the harmonic number
 *          Sum_{i=1..k} 1/i.
 * https://oeis.org/A002387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = floor(exp(n - gamma) + 1/2), a(0) = 1 (Hickerson's
 *            formula; computed in exact multi-limb fixed point, gamma by
 *            Brent-McMillan)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   g_add, g_cmp, g_divk2, g_divs, g_fmul, g_len, g_muladd, g_pow2,
 *            g_shl, g_shr, g_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/g_add.h"
#include "../utils/g_cmp.h"
#include "../utils/g_divk2.h"
#include "../utils/g_divs.h"
#include "../utils/g_fmul.h"
#include "../utils/g_len.h"
#include "../utils/g_muladd.h"
#include "../utils/g_pow2.h"
#include "../utils/g_shl.h"
#include "../utils/g_shr.h"
#include "../utils/g_sub.h"

seq_t a(idx_t n) {
  size_t P, L, i, lv, m;
  uint64_t N, k, ex;
  uint32_t q0;
  uint32_t *ln2, *t, *u, *B, *C, *V, *SC, *Q, *S, *R, *tmp;
  seq_t res;

  if (n == 0) return 1;

  P = (size_t)(n * 1443 / 1000) + 160;
  L = (2 * P + 512) / 32 + 8;

  ln2 = (uint32_t *)malloc(L * sizeof(uint32_t));
  t = (uint32_t *)malloc(L * sizeof(uint32_t));
  u = (uint32_t *)malloc(L * sizeof(uint32_t));
  B = (uint32_t *)malloc(L * sizeof(uint32_t));
  C = (uint32_t *)malloc(L * sizeof(uint32_t));
  V = (uint32_t *)malloc(L * sizeof(uint32_t));
  SC = (uint32_t *)malloc(L * sizeof(uint32_t));
  Q = (uint32_t *)malloc(L * sizeof(uint32_t));
  S = (uint32_t *)malloc(L * sizeof(uint32_t));
  R = (uint32_t *)malloc(L * sizeof(uint32_t));
  tmp = (uint32_t *)malloc((2 * L + 2) * sizeof(uint32_t));

  /* ln 2 = sum_{j odd} 2 / (j * 3^j) */
  memset(ln2, 0, L * sizeof(uint32_t));
  g_pow2(t, L, P + 1);
  g_divs(t, L, 3);
  for (k = 1; g_len(t, L); k += 2) {
    memcpy(u, t, L * sizeof(uint32_t));
    g_divs(u, L, (uint32_t)k);
    g_add(ln2, u, L);
    g_divs(t, L, 9);
  }

  /* Brent-McMillan with N = 2^m, error ~ pi*exp(-4N) */
  m = 0;
  N = 1;
  while (N * 577 < (uint64_t)(P + 10) * 100) {
    N *= 2;
    m++;
  }
  g_pow2(B, L, P);
  memset(C, 0, L * sizeof(uint32_t));
  g_pow2(V, L, P);
  memset(SC, 0, L * sizeof(uint32_t));
  for (k = 1;; k++) {
    g_shl(B, L, 2 * m);
    g_divk2(B, L, k);
    g_shl(C, L, 2 * m);
    g_muladd(C, B, L, (uint32_t)k);
    g_divk2(C, L, k);
    if (!g_len(B, L) && !g_len(C, L)) break;
    g_add(V, B, L);
    g_add(SC, C, L);
  }

  /* Q = SC * 2^P / V  (= gamma + ln N) */
  lv = g_len(V, L) + 1;
  memcpy(R, SC, L * sizeof(uint32_t));
  q0 = 0;
  while (g_cmp(R, V, lv) >= 0) {
    g_sub(R, V, lv);
    q0++;
  }
  memset(u, 0, L * sizeof(uint32_t));
  for (i = P; i-- > 0;) {
    g_shl(R, lv, 1);
    if (g_cmp(R, V, lv) >= 0) {
      g_sub(R, V, lv);
      u[i / 32] |= (uint32_t)1 << (i % 32);
    }
  }
  memset(Q, 0, L * sizeof(uint32_t));
  Q[0] = q0;
  g_shl(Q, L, P);
  g_add(Q, u, L);

  /* gamma = Q - m ln 2 */
  memset(t, 0, L * sizeof(uint32_t));
  g_muladd(t, ln2, L, (uint32_t)m);
  g_sub(Q, t, L);

  /* S = exp(1 - gamma) via exp(y / 2^32)^(2^32) */
  g_pow2(u, L, P);
  g_sub(u, Q, L);
  g_shr(u, u, L, L, 32);
  memset(S, 0, L * sizeof(uint32_t));
  g_pow2(t, L, P);
  for (k = 1; g_len(t, L); k++) {
    g_add(S, t, L);
    g_fmul(t, t, u, L, P, tmp);
    g_divs(t, L, (uint32_t)k);
  }
  for (i = 0; i < 32; i++) g_fmul(S, S, S, L, P, tmp);

  /* e */
  memset(B, 0, L * sizeof(uint32_t));
  g_pow2(t, L, P);
  for (k = 1; g_len(t, L); k++) {
    g_add(B, t, L);
    g_divs(t, L, (uint32_t)k);
  }

  /* R = e^(n-1) */
  g_pow2(R, L, P);
  ex = (uint64_t)n - 1;
  while (ex) {
    if (ex & 1) g_fmul(R, R, B, L, P, tmp);
    ex >>= 1;
    if (ex) g_fmul(B, B, B, L, P, tmp);
  }

  /* round(e^(n-1) * e^(1-gamma)) */
  g_fmul(R, R, S, L, P, tmp);
  g_pow2(t, L, P - 1);
  g_add(R, t, L);
  g_shr(R, R, L, L, P);

  res = 0;
  for (i = g_len(R, L); i-- > 0;) {
    res = res * 256;
    res = res * 256;
    res = res * 256;
    res = res * 256;
    res += (idx_t)R[i];
  }

  free(ln2); free(t); free(u); free(B); free(C); free(V);
  free(SC); free(Q); free(S); free(R); free(tmp);
  return res;
}
