/*
 * A002638  a(n) = (number of nonisomorphic nontransitive prime tournaments on
 *          n nodes) - Moebius(n).
 * https://oeis.org/A002638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = -1, a(n) = A000568(n) - Sum_{d|n, d!=1, d!=n} a(d) *
 *            A000568(n/d), where A000568(n) = Sum over partitions of n into
 *            odd parts p with multiplicity m_p of 2^(Sum_{p<q}
 *            m_p*m_q*gcd(p,q) + Sum_p (p*C(m_p,2) + m_p*(p-1)/2)) / Prod_p
 *            (p^m_p * m_p!)
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 11, i64 n <= 14, i128 n <= 19,
 *            big any n
 * Helpers:   q2638_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/q2638_gcd.h"

typedef struct { uint32_t *d; size_t n; size_t cap; } q2638_big;

static void q2638_reserve(q2638_big *x, size_t c) {
  if (c > x->cap) {
    x->d = (uint32_t *)realloc(x->d, c * sizeof(uint32_t));
    memset(x->d + x->cap, 0, (c - x->cap) * sizeof(uint32_t));
    x->cap = c;
  }
}

static void q2638_trim(q2638_big *x) {
  while (x->n && x->d[x->n - 1] == 0) x->n--;
}

static void q2638_set_small(q2638_big *x, uint32_t v) {
  q2638_reserve(x, 2);
  memset(x->d, 0, x->cap * sizeof(uint32_t));
  x->d[0] = v;
  x->n = v ? 1 : 0;
}

static void q2638_copy(q2638_big *dst, const q2638_big *src) {
  q2638_reserve(dst, src->n + 1);
  if (dst->n > src->n) memset(dst->d + src->n, 0, (dst->n - src->n) * sizeof(uint32_t));
  if (src->n) memcpy(dst->d, src->d, src->n * sizeof(uint32_t));
  dst->n = src->n;
}

static void q2638_free(q2638_big *x) {
  free(x->d);
  x->d = NULL;
  x->n = 0;
  x->cap = 0;
}

/* acc += x << t */
static void q2638_addshift(q2638_big *acc, const q2638_big *x, uint64_t t) {
  size_t w = (size_t)(t / 32);
  unsigned s = (unsigned)(t % 32);
  size_t xl = x->n + 1;
  size_t need = w + xl + 1;
  size_t i;
  uint64_t carry = 0;
  if (acc->n + 1 > need) need = acc->n + 1;
  q2638_reserve(acc, need);
  for (i = 0; i < xl; i++) {
    uint32_t lo = i < x->n ? x->d[i] : 0;
    uint32_t piece;
    if (s) piece = (uint32_t)((lo << s) | (i ? (x->d[i - 1] >> (32 - s)) : 0));
    else piece = lo;
    uint64_t sum = (uint64_t)acc->d[w + i] + piece + carry;
    acc->d[w + i] = (uint32_t)sum;
    carry = sum >> 32;
  }
  for (i = w + xl; carry; i++) {
    uint64_t sum = (uint64_t)acc->d[i] + carry;
    acc->d[i] = (uint32_t)sum;
    carry = sum >> 32;
  }
  acc->n = need;
  q2638_trim(acc);
}

static void q2638_mulsmall(q2638_big *x, uint32_t m) {
  uint64_t c = 0;
  size_t i;
  for (i = 0; i < x->n; i++) {
    uint64_t v = (uint64_t)x->d[i] * m + c;
    x->d[i] = (uint32_t)v;
    c = v >> 32;
  }
  if (c) {
    q2638_reserve(x, x->n + 2);
    x->d[x->n++] = (uint32_t)c;
  }
}

static void q2638_divsmall(q2638_big *x, uint32_t m) {
  uint64_t r = 0;
  size_t i;
  if (m == 1) return;
  for (i = x->n; i-- > 0;) {
    uint64_t cur = (r << 32) | x->d[i];
    x->d[i] = (uint32_t)(cur / m);
    r = cur % m;
  }
  q2638_trim(x);
}

static void q2638_mul(const q2638_big *a, const q2638_big *b, q2638_big *out) {
  size_t i, j;
  q2638_set_small(out, 0);
  if (a->n == 0 || b->n == 0) return;
  q2638_reserve(out, a->n + b->n + 1);
  for (i = 0; i < a->n; i++) {
    uint64_t c = 0;
    for (j = 0; j < b->n; j++) {
      uint64_t v = (uint64_t)a->d[i] * b->d[j] + out->d[i + j] + c;
      out->d[i + j] = (uint32_t)v;
      c = v >> 32;
    }
    size_t k = i + b->n;
    while (c) {
      uint64_t v = (uint64_t)out->d[k] + c;
      out->d[k] = (uint32_t)v;
      c = v >> 32;
      k++;
    }
  }
  out->n = a->n + b->n + 1;
  q2638_trim(out);
}

static int q2638_cmp(const q2638_big *a, const q2638_big *b) {
  size_t i;
  if (a->n != b->n) return a->n < b->n ? -1 : 1;
  for (i = a->n; i-- > 0;) {
    if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
  }
  return 0;
}

/* a -= b, requires a >= b */
static void q2638_sub(q2638_big *a, const q2638_big *b) {
  int64_t borrow = 0;
  size_t i;
  for (i = 0; i < a->n; i++) {
    int64_t v = (int64_t)a->d[i] - (i < b->n ? (int64_t)b->d[i] : 0) - borrow;
    if (v < 0) { v += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
    a->d[i] = (uint32_t)v;
  }
  q2638_trim(a);
}

static q2638_big q2638_S;
static q2638_big *q2638_coef;
static int *q2638_pp;
static int *q2638_pm;

static void q2638_rec(int depth, int rem, int maxp, uint64_t t) {
  int p, k, m;
  if (rem == 0) {
    q2638_addshift(&q2638_S, &q2638_coef[depth], t);
    return;
  }
  if (maxp > rem) maxp = rem;
  if (maxp % 2 == 0) maxp--;
  for (p = maxp; p >= 1; p -= 2) {
    uint64_t cross = 0;
    for (k = 0; k < depth; k++)
      cross += (uint64_t)q2638_pm[k] * q2638_gcd((unsigned)p, (unsigned)q2638_pp[k]);
    q2638_copy(&q2638_coef[depth + 1], &q2638_coef[depth]);
    q2638_pp[depth] = p;
    if (p == 1) {
      for (m = 1; m <= rem; m++) q2638_divsmall(&q2638_coef[depth + 1], (uint32_t)m);
      m = rem;
      q2638_pm[depth] = m;
      q2638_rec(depth + 1, 0, -1,
                t + (uint64_t)m * (uint64_t)(m - 1) / 2 + (uint64_t)m * cross);
      continue;
    }
    for (m = 1; m * p <= rem; m++) {
      uint64_t tn;
      q2638_divsmall(&q2638_coef[depth + 1], (uint32_t)p);
      q2638_divsmall(&q2638_coef[depth + 1], (uint32_t)m);
      tn = t + (uint64_t)p * (uint64_t)m * (uint64_t)(m - 1) / 2
             + (uint64_t)m * (uint64_t)(p - 1) / 2 + (uint64_t)m * cross;
      q2638_pm[depth] = m;
      q2638_rec(depth + 1, rem - m * p, p - 2, tn);
    }
  }
}

/* out = A000568(n) */
static void q2638_tournaments(int n, q2638_big *out) {
  int i;
  q2638_coef = (q2638_big *)calloc((size_t)n + 2, sizeof(q2638_big));
  q2638_pp = (int *)calloc((size_t)n + 2, sizeof(int));
  q2638_pm = (int *)calloc((size_t)n + 2, sizeof(int));
  q2638_set_small(&q2638_S, 0);
  q2638_set_small(&q2638_coef[0], 1);
  for (i = 2; i <= n; i++) q2638_mulsmall(&q2638_coef[0], (uint32_t)i);
  q2638_rec(0, n, n, 0);
  for (i = 2; i <= n; i++) q2638_divsmall(&q2638_S, (uint32_t)i);
  q2638_copy(out, &q2638_S);
  for (i = 0; i < n + 2; i++) q2638_free(&q2638_coef[i]);
  free(q2638_coef);
  free(q2638_pp);
  free(q2638_pm);
  q2638_coef = NULL;
}

static q2638_big *q2638_T = NULL;
static q2638_big *q2638_A = NULL;
static int *q2638_Asign = NULL;
static unsigned char *q2638_hasT = NULL;
static unsigned char *q2638_hasA = NULL;
static size_t q2638_cap = 0;

static void q2638_ensure(size_t n) {
  size_t nc = n + 1;
  if (nc <= q2638_cap) return;
  q2638_T = (q2638_big *)realloc(q2638_T, nc * sizeof(q2638_big));
  q2638_A = (q2638_big *)realloc(q2638_A, nc * sizeof(q2638_big));
  q2638_Asign = (int *)realloc(q2638_Asign, nc * sizeof(int));
  q2638_hasT = (unsigned char *)realloc(q2638_hasT, nc);
  q2638_hasA = (unsigned char *)realloc(q2638_hasA, nc);
  memset(q2638_T + q2638_cap, 0, (nc - q2638_cap) * sizeof(q2638_big));
  memset(q2638_A + q2638_cap, 0, (nc - q2638_cap) * sizeof(q2638_big));
  memset(q2638_Asign + q2638_cap, 0, (nc - q2638_cap) * sizeof(int));
  memset(q2638_hasT + q2638_cap, 0, nc - q2638_cap);
  memset(q2638_hasA + q2638_cap, 0, nc - q2638_cap);
  q2638_cap = nc;
}

static void q2638_getT(size_t k) {
  if (q2638_hasT[k]) return;
  q2638_tournaments((int)k, &q2638_T[k]);
  q2638_hasT[k] = 1;
}

static void q2638_getA(size_t n) {
  q2638_big pos, neg, prod;
  size_t d;
  if (q2638_hasA[n]) return;
  if (n == 1) {
    q2638_set_small(&q2638_A[1], 1);
    q2638_Asign[1] = -1;
    q2638_hasA[1] = 1;
    return;
  }
  memset(&pos, 0, sizeof(pos));
  memset(&neg, 0, sizeof(neg));
  memset(&prod, 0, sizeof(prod));
  q2638_getT(n);
  q2638_copy(&pos, &q2638_T[n]);
  q2638_set_small(&neg, 0);
  for (d = 2; d < n; d++) {
    if (n % d != 0) continue;
    q2638_getA(d);
    q2638_getT(n / d);
    q2638_mul(&q2638_A[d], &q2638_T[n / d], &prod);
    if (q2638_Asign[d] < 0) q2638_addshift(&pos, &prod, 0);
    else q2638_addshift(&neg, &prod, 0);
  }
  if (q2638_cmp(&pos, &neg) >= 0) {
    q2638_sub(&pos, &neg);
    q2638_copy(&q2638_A[n], &pos);
    q2638_Asign[n] = 1;
  } else {
    q2638_sub(&neg, &pos);
    q2638_copy(&q2638_A[n], &neg);
    q2638_Asign[n] = -1;
  }
  q2638_hasA[n] = 1;
  q2638_free(&pos);
  q2638_free(&neg);
  q2638_free(&prod);
}

seq_t a(idx_t n) {
  seq_t v = 0;
  size_t i;
  const q2638_big *m;
  if (n < 1) return v;
  q2638_ensure((size_t)n);
  q2638_getA((size_t)n);
  m = &q2638_A[n];
  for (i = m->n; i-- > 0;) {
    v = v * 65536;
    v = v * 65536;
    v += (idx_t)m->d[i];
  }
  if (q2638_Asign[n] < 0) v = -v;
  return v;
}
