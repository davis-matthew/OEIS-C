/*
 * A002073  Numerators of coefficients in an asymptotic expansion of the
 *          confluent hypergeometric function F(1-b; 2; 4b).
 * https://oeis.org/A002073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = numerator(P_n) where P(y) = H(y)^(-(2n+1)/3), H(y) =
 *            Sum_{k>=0} 3/(2k+3) y^k (Lagrange inversion: (2n+1)*[x^(2n+1)] g
 *            = [x^(2n)] (x/f(x))^(2n+1), f^3 = 3*(artanh(x) - x)). P_0 = 1,
 *            k*P_k = Sum_{j=1..k} ((alpha+1)*j - k) * H_j * P_{k-j}, alpha =
 *            -(2n+1)/3, evaluated exactly with integer num...
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 9, i64 n <= 13, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t n, cap; } BN;

static void bn_reserve(BN *x, size_t c) {
  if (c > x->cap) {
    size_t nc = 2 * c + 4;
    x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
    x->cap = nc;
  }
}

static void bn_norm(BN *x) {
  while (x->n && x->d[x->n - 1] == 0) x->n--;
}

static void bn_copy(BN *x, const BN *y) {
  bn_reserve(x, y->n);
  if (y->n) memcpy(x->d, y->d, y->n * sizeof(uint32_t));
  x->n = y->n;
}

static void bn_muls(BN *x, uint32_t m) {
  uint64_t c = 0;
  size_t i;
  if (m == 0) { x->n = 0; return; }
  for (i = 0; i < x->n; i++) {
    uint64_t t = (uint64_t)x->d[i] * m + c;
    x->d[i] = (uint32_t)t;
    c = t >> 32;
  }
  if (c) {
    bn_reserve(x, x->n + 1);
    x->d[x->n++] = (uint32_t)c;
  }
}

static void bn_add(BN *x, const BN *y) {
  size_t L = x->n > y->n ? x->n : y->n, i;
  uint64_t c = 0;
  bn_reserve(x, L + 1);
  for (i = x->n; i < L; i++) x->d[i] = 0;
  for (i = 0; i < L; i++) {
    uint64_t t = (uint64_t)x->d[i] + (i < y->n ? y->d[i] : 0) + c;
    x->d[i] = (uint32_t)t;
    c = t >> 32;
  }
  x->n = L;
  if (c) x->d[x->n++] = (uint32_t)c;
}

static int bn_cmp(const BN *x, const BN *y) {
  size_t i;
  if (x->n != y->n) return x->n < y->n ? -1 : 1;
  for (i = x->n; i-- > 0;)
    if (x->d[i] != y->d[i]) return x->d[i] < y->d[i] ? -1 : 1;
  return 0;
}

/* x -= y, requires x >= y */
static void bn_sub(BN *x, const BN *y) {
  int64_t b = 0;
  size_t i;
  for (i = 0; i < x->n; i++) {
    int64_t t = (int64_t)x->d[i] - (i < y->n ? (int64_t)y->d[i] : 0) - b;
    if (t < 0) { t += ((int64_t)1 << 32); b = 1; } else b = 0;
    x->d[i] = (uint32_t)t;
  }
  bn_norm(x);
}

static uint32_t bn_mod(const BN *x, uint32_t m) {
  uint64_t r = 0;
  size_t i;
  for (i = x->n; i-- > 0;) r = ((r << 32) | x->d[i]) % m;
  return (uint32_t)r;
}

static void bn_div(BN *x, uint32_t m) {
  uint64_t r = 0;
  size_t i;
  for (i = x->n; i-- > 0;) {
    uint64_t cur = (r << 32) | x->d[i];
    x->d[i] = (uint32_t)(cur / m);
    r = cur % m;
  }
  bn_norm(x);
}

static void pack_mul(BN *x, uint64_t *acc, uint64_t f) {
  if (*acc * f > 0xFFFFFFFFull) {
    bn_muls(x, (uint32_t)*acc);
    *acc = f;
  } else {
    *acc *= f;
  }
}

seq_t a(idx_t n) {
  size_t N = (size_t)n;
  size_t PM = 2 * N + 3;
  char *comp = (char *)calloc(PM + 1, 1);
  uint32_t *pr = (uint32_t *)malloc((PM + 1) * sizeof(uint32_t));
  size_t np = 0, i, q, k, j;

  for (i = 2; i <= PM; i++) {
    if (!comp[i]) {
      size_t s;
      pr[np++] = (uint32_t)i;
      for (s = i * i; s <= PM; s += i) comp[s] = 1;
    }
  }

  /* E[q*(N+1)+k] = exponent of prime pr[q] in F_k */
  uint16_t *E = (uint16_t *)calloc(np * (N + 1), sizeof(uint16_t));
  for (q = 0; q < np; q++) {
    size_t p = pr[q], j0;
    uint16_t *Eq = E + q * (N + 1);
    if (p == 2) continue;
    j0 = (p - 3) / 2;
    if (j0 == 0) j0 = p;
    for (k = 1; k <= N; k++) {
      unsigned best = Eq[k - 1];
      for (j = j0; j <= k; j += p) {
        size_t w = 2 * j + 3;
        unsigned v = 0;
        while (w % p == 0) { w /= p; v++; }
        if (Eq[k - j] + v > best) best = Eq[k - j] + v;
      }
      Eq[k] = (uint16_t)best;
    }
  }

  BN *Nk = (BN *)calloc(N + 1, sizeof(BN));
  int *sg = (int *)calloc(N + 1, sizeof(int));
  BN pos = {0, 0, 0}, neg = {0, 0, 0}, t = {0, 0, 0};

  bn_reserve(&Nk[0], 1);
  Nk[0].d[0] = 1;
  Nk[0].n = 1;

  for (k = 1; k <= N; k++) {
    pos.n = 0;
    neg.n = 0;
    for (j = 1; j <= k; j++) {
      uint64_t acc = 1;
      uint64_t dj = 3 * (uint64_t)k + (2 * (uint64_t)N - 2) * (uint64_t)j;
      size_t w = 2 * j + 3, ii;
      if (Nk[k - j].n == 0) continue;
      bn_copy(&t, &Nk[k - j]);
      pack_mul(&t, &acc, dj);
      for (ii = k - j + 1; ii < k; ii++) pack_mul(&t, &acc, ii);
      for (q = 0; q < np && pr[q] <= 2 * k + 3; q++) {
        size_t p = pr[q];
        long x;
        if (p == 2) continue;
        x = (long)E[q * (N + 1) + k] - (long)E[q * (N + 1) + (k - j)];
        if (w % p == 0) {
          size_t ww = w;
          while (ww % p == 0) { ww /= p; x--; }
        }
        while (x-- > 0) pack_mul(&t, &acc, p);
      }
      if (acc > 1) bn_muls(&t, (uint32_t)acc);
      /* coefficient c_j = -dj is negative */
      if (sg[k - j]) bn_add(&pos, &t);
      else bn_add(&neg, &t);
    }
    if (bn_cmp(&pos, &neg) >= 0) {
      bn_sub(&pos, &neg);
      bn_copy(&Nk[k], &pos);
      sg[k] = 0;
    } else {
      bn_sub(&neg, &pos);
      bn_copy(&Nk[k], &neg);
      sg[k] = 1;
    }
  }

  /* reduce N_n / (n! * F_n) */
  BN *R = &Nk[N];
  for (q = 0; q < np && R->n; q++) {
    size_t p = pr[q], m;
    long cap = 0;
    for (m = N / p; m > 0; m /= p) cap += (long)m;
    if (p != 2) cap += E[q * (N + 1) + N];
    while (cap > 0 && bn_mod(R, (uint32_t)p) == 0) {
      bn_div(R, (uint32_t)p);
      cap--;
    }
  }

  seq_t r = 0;
  int isneg = sg[N];
  for (i = R->n; i-- > 0;) {
    uint32_t wv = R->d[i];
    idx_t hi = (idx_t)(wv >> 16), lo = (idx_t)(wv & 0xFFFFu);
    if (isneg) {
      r = r * 65536 - hi;
      r = r * 65536 - lo;
    } else {
      r = r * 65536 + hi;
      r = r * 65536 + lo;
    }
  }

  for (k = 0; k <= N; k++) free(Nk[k].d);
  free(Nk);
  free(sg);
  free(pos.d);
  free(neg.d);
  free(t.d);
  free(E);
  free(pr);
  free(comp);
  return r;
}
