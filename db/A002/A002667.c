/*
 * A002667  Continued cotangent for Pi.
 * https://oeis.org/A002667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Lehmer's continued cotangent: x(0) = Pi, a(n) = floor(x(n)),
 *            x(n+1) = (a(n)*x(n) + 1)/(x(n) - a(n)). Pi is computed with
 *            Machin's formula to adaptive precision, and the recurrence runs
 *            on rational interval bounds until both endpoints give the same
 *            a(k) for k = 0..n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 10 terms through n = 9 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void c2667_add(uint32_t *x, const uint32_t *y, size_t L) {
  uint64_t c = 0;
  size_t i;
  for (i = 0; i < L; i++) {
    c += (uint64_t)x[i] + y[i];
    x[i] = (uint32_t)c;
    c >>= 32;
  }
}

static void c2667_addw(uint32_t *x, size_t L, uint32_t v) {
  uint64_t c = v;
  size_t i;
  for (i = 0; i < L && c; i++) {
    c += x[i];
    x[i] = (uint32_t)c;
    c >>= 32;
  }
}

static void c2667_divs(uint32_t *x, size_t L, uint32_t d) {
  uint64_t r = 0;
  size_t i = L;
  while (i-- > 0) {
    uint64_t cur = (r << 32) | x[i];
    x[i] = (uint32_t)(cur / d);
    r = cur % d;
  }
}

static int c2667_iszero(const uint32_t *x, size_t L) {
  size_t i;
  for (i = 0; i < L; i++) if (x[i]) return 0;
  return 1;
}

static void c2667_sub(uint32_t *x, const uint32_t *y, size_t L) {
  uint64_t br = 0;
  size_t i;
  for (i = 0; i < L; i++) {
    uint64_t d = (uint64_t)x[i] - y[i] - br;
    x[i] = (uint32_t)d;
    br = (d >> 63) & 1;
  }
}

/* out = floor(atan(1/x) * 2^sh) approximately (error a few units) */
static void c2667_atan(uint32_t *out, uint32_t *pos, uint32_t *neg, uint32_t *t,
                       uint32_t *u, size_t L, uint32_t x, size_t sh) {
  uint32_t k;
  memset(pos, 0, L * sizeof(uint32_t));
  memset(neg, 0, L * sizeof(uint32_t));
  memset(t, 0, L * sizeof(uint32_t));
  t[sh / 32] = (uint32_t)1 << (sh % 32);
  c2667_divs(t, L, x);
  for (k = 0; !c2667_iszero(t, L); k++) {
    memcpy(u, t, L * sizeof(uint32_t));
    c2667_divs(u, L, 2 * k + 1);
    c2667_add((k & 1) ? neg : pos, u, L);
    c2667_divs(t, L, x * x);
  }
  memcpy(out, pos, L * sizeof(uint32_t));
  c2667_sub(out, neg, L);
}

static size_t c2667_bl(const uint32_t *x, size_t L) {
  size_t i = L, b;
  uint32_t t;
  while (i > 0 && x[i - 1] == 0) i--;
  if (i == 0) return 0;
  t = x[i - 1];
  b = (i - 1) * 32;
  while (t) { b++; t >>= 1; }
  return b;
}

static int c2667_cmp(const uint32_t *x, const uint32_t *y, size_t L) {
  size_t i = L;
  while (i-- > 0) {
    if (x[i] != y[i]) return x[i] > y[i] ? 1 : -1;
  }
  return 0;
}

static void c2667_shl(uint32_t *x, size_t L, size_t s) {
  size_t w = s / 32, i;
  unsigned b = (unsigned)(s % 32);
  if (w) for (i = L; i-- > 0;) x[i] = i >= w ? x[i - w] : 0;
  if (b) for (i = L; i-- > 0;) x[i] = (x[i] << b) | (i ? x[i - 1] >> (32 - b) : 0);
}

static void c2667_shr(uint32_t *x, size_t L, size_t s) {
  size_t w = s / 32, i;
  unsigned b = (unsigned)(s % 32);
  if (w) for (i = 0; i < L; i++) x[i] = i + w < L ? x[i + w] : 0;
  if (b) for (i = 0; i < L; i++) x[i] = (x[i] >> b) | (i + 1 < L ? x[i + 1] << (32 - b) : 0);
}

/* A = floor(P/Q); (P,Q) <- (A*P + Q, P - A*Q). Returns 0 on failure. */
static int c2667_step(uint32_t *P, uint32_t *Q, uint32_t *A, uint32_t *R,
                      uint32_t *D, uint32_t *T, size_t L) {
  size_t bp = c2667_bl(P, L), bq = c2667_bl(Q, L), ba, s, i;
  memset(A, 0, L * sizeof(uint32_t));
  if (bq == 0 || bp < bq) return 0;
  memcpy(R, P, L * sizeof(uint32_t));
  memcpy(D, Q, L * sizeof(uint32_t));
  c2667_shl(D, L, bp - bq);
  for (i = bp - bq + 1; i-- > 0;) {
    if (c2667_cmp(R, D, L) >= 0) {
      c2667_sub(R, D, L);
      A[i / 32] |= (uint32_t)1 << (i % 32);
    }
    c2667_shr(D, L, 1);
  }
  if (c2667_iszero(R, L)) return 0;
  ba = c2667_bl(A, L);
  if (ba + bp + 2 >= L * 32) return 0;
  memcpy(D, Q, L * sizeof(uint32_t));
  memcpy(T, P, L * sizeof(uint32_t));
  for (s = 0; s < ba; s++) {
    if ((A[s / 32] >> (s % 32)) & 1) c2667_add(D, T, L);
    c2667_shl(T, L, 1);
  }
  memcpy(P, D, L * sizeof(uint32_t));
  memcpy(Q, R, L * sizeof(uint32_t));
  return 1;
}

static void c2667_subw(uint32_t *x, size_t L, uint32_t v) {
  uint64_t br = v;
  size_t i;
  for (i = 0; i < L && br; i++) {
    uint64_t d = (uint64_t)x[i] - br;
    x[i] = (uint32_t)d;
    br = (d >> 63) & 1;
  }
}

seq_t a(idx_t n) {
  size_t N = 64 + ((size_t)8 << (n < 40 ? n : 40));
  for (;;) {
    size_t L = 3 * N / 32 + 8, G = 64, i;
    uint32_t *blk = (uint32_t *)calloc(9 * L, sizeof(uint32_t));
    uint32_t *lP = blk, *lQ = blk + L, *uP = blk + 2 * L, *uQ = blk + 3 * L;
    uint32_t *aL = blk + 4 * L, *aU = blk + 5 * L, *R = blk + 6 * L;
    uint32_t *D = blk + 7 * L, *T = blk + 8 * L;
    idx_t k;
    int ok = 1;

    c2667_atan(lP, R, D, T, aL, L, 5, N + G);
    c2667_atan(uP, R, D, T, aL, L, 239, N + G);
    c2667_shl(lP, L, 4);
    c2667_shl(uP, L, 2);
    c2667_sub(lP, uP, L);
    c2667_shr(lP, L, G);
    memcpy(uP, lP, L * sizeof(uint32_t));
    c2667_subw(lP, L, 1);
    c2667_addw(uP, L, 2);
    memset(lQ, 0, L * sizeof(uint32_t));
    lQ[N / 32] = (uint32_t)1 << (N % 32);
    memcpy(uQ, lQ, L * sizeof(uint32_t));

    for (k = 0; k <= n; k++) {
      if (!c2667_step(lP, lQ, aL, R, D, T, L) ||
          !c2667_step(uP, uQ, aU, R, D, T, L) ||
          c2667_cmp(aL, aU, L) != 0) {
        ok = 0;
        break;
      }
    }
    if (ok) {
      seq_t res = 0;
      size_t top = L;
      while (top > 0 && aL[top - 1] == 0) top--;
      for (i = top; i-- > 0;) {
        int j;
        for (j = 3; j >= 0; j--) {
          res *= 256;
          res += (seq_t)((aL[i] >> (8 * j)) & 255u);
        }
      }
      free(blk);
      return res;
    }
    free(blk);
    N *= 2;
  }
}
