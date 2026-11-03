/*
 * A003326  Nearest integer to Bernoulli(2n)/(-4n).
 * https://oeis.org/A003326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = round(B(2n)/(-4n)) = round((-1)^n * T(n) /
 *            (2^(2n+1)*(4^n-1))), where T(n) = A000182(n) are the tangent
 *            numbers, using B(2n) = (-1)^(n-1)*2n*T(n)/(4^n*(4^n-1)). T(n) is
 *            computed exactly with the Brent-Zimmermann recurrence T(j) <-
 *            (j-k)*T(j-1) + (j-k+2)*T(j)
 * Range:     i8 n <= 10, i16 n <= 12, i32 n <= 16, i64 n <= 22, i128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* x += y; returns new length (x must have room) */
static size_t a3326_add(uint32_t *x, size_t xl, const uint32_t *y, size_t yl) {
  size_t L = xl > yl ? xl : yl;
  uint64_t carry = 0;
  for (size_t i = 0; i < L; i++) {
    uint64_t v = carry;
    if (i < xl) v += x[i];
    if (i < yl) v += y[i];
    x[i] = (uint32_t)v;
    carry = v >> 32;
  }
  if (carry) x[L++] = (uint32_t)carry;
  return L;
}

/* is x == 2^s - 1 ? */
static int a3326_allones(const uint32_t *x, size_t len, size_t s) {
  size_t full = (s + 31) / 32;
  if (len != full || s == 0) return 0;
  for (size_t i = 0; i + 1 < len; i++)
    if (x[i] != 0xFFFFFFFFu) return 0;
  uint32_t top = (s % 32) ? (uint32_t)((1u << (s % 32)) - 1) : 0xFFFFFFFFu;
  return x[len - 1] == top;
}

static size_t a3326_bitlen(const uint32_t *x, size_t len) {
  if (!len) return 0;
  uint32_t t = x[len - 1];
  size_t b = 0;
  while (t) { b++; t >>= 1; }
  return (len - 1) * 32 + b;
}

static size_t a3326_norm(const uint32_t *x, size_t len) {
  while (len && !x[len - 1]) len--;
  return len;
}

/* y = a*x + b*y */
static size_t a3326_lincomb(uint32_t *y, size_t yl, const uint32_t *x, size_t xl,
                            uint64_t ca, uint64_t cb) {
  size_t L = xl > yl ? xl : yl;
  uint64_t carry = 0;
  for (size_t i = 0; i < L; i++) {
    uint64_t v = carry;
    if (i < xl) v += ca * x[i];
    if (i < yl) v += cb * y[i];
    y[i] = (uint32_t)v;
    carry = v >> 32;
  }
  if (carry) y[L++] = (uint32_t)carry;
  return a3326_norm(y, L);
}

/* dst = src mod 2^s */
static size_t a3326_mask(uint32_t *dst, const uint32_t *src, size_t len, size_t s) {
  size_t full = (s + 31) / 32;
  size_t L = len < full ? len : full;
  for (size_t i = 0; i < L; i++) dst[i] = src[i];
  if (L == full && s % 32) dst[L - 1] &= (uint32_t)((1u << (s % 32)) - 1);
  return a3326_norm(dst, L);
}

/* dst = src >> s; dst may alias src */
static size_t a3326_shr(uint32_t *dst, const uint32_t *src, size_t len, size_t s) {
  size_t w = s / 32, b = s % 32;
  if (w >= len) return 0;
  size_t L = len - w;
  for (size_t i = 0; i < L; i++) {
    uint64_t v = src[i + w];
    if (b) {
      v >>= b;
      if (i + w + 1 < len) v |= (uint64_t)src[i + w + 1] << (32 - b);
    }
    dst[i] = (uint32_t)v;
  }
  return a3326_norm(dst, L);
}

static uint32_t **a3326_tab = NULL;
static size_t *a3326_tlen = NULL;
static idx_t a3326_tmax = 0;

static void a3326_build(idx_t N) {
  if (a3326_tab) {
    for (idx_t j = 0; j <= a3326_tmax; j++) free(a3326_tab[j]);
    free(a3326_tab);
    free(a3326_tlen);
  }
  a3326_tab = (uint32_t **)calloc((size_t)N + 1, sizeof(uint32_t *));
  a3326_tlen = (size_t *)calloc((size_t)N + 1, sizeof(size_t));
  uint64_t bits = 0;
  idx_t i = 1;
  for (idx_t j = 0; j <= N; j++) {
    while (i <= 2 * j) {
      idx_t t = i;
      while (t) { bits++; t >>= 1; }
      i++;
    }
    size_t cap = (size_t)(bits / 32) + 3;
    a3326_tab[j] = (uint32_t *)calloc(cap, sizeof(uint32_t));
  }
  a3326_tab[1][0] = 1;
  a3326_tlen[1] = 1;
  for (idx_t k = 2; k <= N; k++)
    a3326_tlen[k] = a3326_lincomb(a3326_tab[k], 0, a3326_tab[k - 1],
                                  a3326_tlen[k - 1], (uint64_t)(k - 1), 0);
  for (idx_t k = 2; k <= N; k++)
    for (idx_t j = k; j <= N; j++)
      a3326_tlen[j] = a3326_lincomb(a3326_tab[j], a3326_tlen[j], a3326_tab[j - 1],
                                    a3326_tlen[j - 1], (uint64_t)(j - k),
                                    (uint64_t)(j - k + 2));
  a3326_tmax = N;
}

seq_t a(idx_t n) {
  if (n > a3326_tmax) {
    idx_t N = 2 * a3326_tmax;
    if (N < n) N = n;
    if (N < 16) N = 16;
    a3326_build(N);
  }
  const uint32_t *T = a3326_tab[n];
  size_t tl = a3326_tlen[n];
  size_t s = (size_t)(2 * n);

  uint32_t *X = (uint32_t *)calloc(tl + 2, sizeof(uint32_t));
  uint32_t *A = (uint32_t *)calloc(tl + 2, sizeof(uint32_t));
  uint32_t *B = (uint32_t *)calloc(tl + 2, sizeof(uint32_t));
  uint32_t *Q = (uint32_t *)calloc(tl + 2, sizeof(uint32_t));
  size_t ql = 0;

  /* H = floor(T / 2^(2n)) */
  size_t xl = a3326_shr(X, T, tl, s);

  /* Z = floor(H / (2^s - 1)) */
  while (a3326_bitlen(X, xl) > s) {
    size_t al = a3326_shr(A, X, xl, s);
    size_t bl = a3326_mask(B, X, xl, s);
    ql = a3326_add(Q, ql, A, al);
    memcpy(X, B, bl * sizeof(uint32_t));
    xl = a3326_add(X, bl, A, al);
  }
  if (a3326_allones(X, xl, s)) {
    uint32_t one = 1;
    ql = a3326_add(Q, ql, &one, 1);
  }

  seq_t r = 0;
  for (size_t i = ql; i > 0; i--) {
    uint32_t limb = Q[i - 1];
    r = r * 65536 + (seq_t)(limb >> 16);
    r = r * 65536 + (seq_t)(limb & 0xFFFFu);
  }
  free(X); free(A); free(B); free(Q);

  /* round(T/D) = floor((Z+1)/2) */
  r = (r + 1) / 2;
  if (n & 1) r = -r;
  return r;
}
