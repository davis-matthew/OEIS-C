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
 * Validated: standard, pass, 45 terms through n = 1945 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static size_t g_len(const uint32_t *x, size_t L) {
  while (L && !x[L - 1]) L--;
  return L;
}

static void g_add(uint32_t *a, const uint32_t *b, size_t L) {
  size_t nb = g_len(b, L), i;
  uint64_t c = 0;
  for (i = 0; i < nb; i++) {
    c += (uint64_t)a[i] + b[i];
    a[i] = (uint32_t)c;
    c >>= 32;
  }
  for (; c && i < L; i++) {
    c += a[i];
    a[i] = (uint32_t)c;
    c >>= 32;
  }
}

static int g_cmp(const uint32_t *a, const uint32_t *b, size_t n) {
  for (size_t i = n; i-- > 0;)
    if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
  return 0;
}

static void g_divs(uint32_t *x, size_t L, uint32_t d) {
  uint64_t r = 0;
  size_t i = g_len(x, L);
  while (i-- > 0) {
    uint64_t cur = (r << 32) | x[i];
    x[i] = (uint32_t)(cur / d);
    r = cur % d;
  }
}

static void g_divk2(uint32_t *x, size_t L, uint64_t k) {
  if (k < 65536) {
    g_divs(x, L, (uint32_t)(k * k));
  } else {
    g_divs(x, L, (uint32_t)k);
    g_divs(x, L, (uint32_t)k);
  }
}

static void g_shr(uint32_t *d, const uint32_t *s, size_t Ls, size_t Ld, size_t sh) {
  size_t q = sh / 32;
  unsigned r = (unsigned)(sh % 32);
  for (size_t i = 0; i < Ld; i++) {
    size_t j = i + q;
    uint32_t lo = j < Ls ? s[j] : 0;
    uint32_t hi = j + 1 < Ls ? s[j + 1] : 0;
    d[i] = r ? ((lo >> r) | (hi << (32 - r))) : lo;
  }
}

/* out = x*y / 2^P */
static void g_fmul(uint32_t *out, const uint32_t *x, const uint32_t *y,
                   size_t L, size_t P, uint32_t *tmp) {
  size_t la = g_len(x, L), lb = g_len(y, L), i, j;
  memset(tmp, 0, (la + lb + 1) * sizeof(uint32_t));
  for (i = 0; i < la; i++) {
    uint64_t c = 0, xi = x[i];
    if (!xi) continue;
    for (j = 0; j < lb; j++) {
      c += xi * y[j] + tmp[i + j];
      tmp[i + j] = (uint32_t)c;
      c >>= 32;
    }
    tmp[i + lb] = (uint32_t)c;
  }
  g_shr(out, tmp, la + lb, L, P);
}

static void g_muladd(uint32_t *a, const uint32_t *b, size_t L, uint32_t m) {
  size_t nb = g_len(b, L), i;
  uint64_t c = 0;
  for (i = 0; i < nb; i++) {
    c += (uint64_t)b[i] * m + a[i];
    a[i] = (uint32_t)c;
    c >>= 32;
  }
  for (; c && i < L; i++) {
    c += a[i];
    a[i] = (uint32_t)c;
    c >>= 32;
  }
}

static void g_pow2(uint32_t *x, size_t L, size_t e) {
  memset(x, 0, L * sizeof(uint32_t));
  x[e / 32] = (uint32_t)1 << (e % 32);
}

static void g_shl(uint32_t *x, size_t L, size_t s) {
  size_t q = s / 32, n0 = g_len(x, L), top;
  unsigned r = (unsigned)(s % 32);
  if (!n0) return;
  top = n0 + q + 1;
  if (top > L) top = L;
  for (size_t i = top; i-- > 0;) {
    uint32_t v = 0;
    if (i >= q) {
      v = x[i - q] << r;
      if (r && i > q) v |= x[i - q - 1] >> (32 - r);
    }
    x[i] = v;
  }
}

static void g_sub(uint32_t *a, const uint32_t *b, size_t n) {
  uint64_t br = 0;
  for (size_t i = 0; i < n; i++) {
    uint64_t t = (uint64_t)a[i] - (uint64_t)b[i] - br;
    a[i] = (uint32_t)t;
    br = (t >> 32) ? 1 : 0;
  }
}

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
