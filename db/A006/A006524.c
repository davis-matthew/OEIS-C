/*
 * A006524  Egyptian fraction for 1/ Pi.
 * https://oeis.org/A006524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   greedy Egyptian fraction: a(n) = ceiling(1/(1/Pi -
 *            Sum_{k=1..n-1} 1/a(k))). Pi is computed with Machin's formula to
 *            a precision that is checked by interval bounds; precision is
 *            increased until both bounds give the same ceiling
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t n, cap; } bn_t;

static void bn_reserve(bn_t *a, size_t c) {
  if (a->cap < c) { a->d = (uint32_t *)realloc(a->d, c * sizeof(uint32_t)); a->cap = c; }
}
static void bn_trim(bn_t *a) { while (a->n > 0 && a->d[a->n - 1] == 0) a->n--; }
static void bn_free(bn_t *a) { free(a->d); a->d = NULL; a->n = 0; a->cap = 0; }
static void bn_take(bn_t *r, uint32_t *buf, size_t n, size_t cap) {
  free(r->d); r->d = buf; r->n = n; r->cap = cap; bn_trim(r);
}
static void bn_set(bn_t *r, uint32_t v) { bn_reserve(r, 1); r->d[0] = v; r->n = v ? 1 : 0; }
static void bn_copy(bn_t *r, const bn_t *a) {
  if (r == a) return;
  bn_reserve(r, a->n + 1);
  if (a->n) memcpy(r->d, a->d, a->n * sizeof(uint32_t));
  r->n = a->n;
}
static int bn_cmp(const bn_t *a, const bn_t *b) {
  if (a->n != b->n) return a->n < b->n ? -1 : 1;
  for (size_t i = a->n; i-- > 0;)
    if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
  return 0;
}
static void bn_add(bn_t *r, const bn_t *a, const bn_t *b) {
  size_t m = a->n > b->n ? a->n : b->n;
  uint32_t *buf = (uint32_t *)malloc((m + 1) * sizeof(uint32_t));
  uint64_t c = 0;
  for (size_t i = 0; i < m; i++) {
    c += (uint64_t)(i < a->n ? a->d[i] : 0) + (uint64_t)(i < b->n ? b->d[i] : 0);
    buf[i] = (uint32_t)c; c >>= 32;
  }
  buf[m] = (uint32_t)c;
  bn_take(r, buf, m + 1, m + 1);
}
/* requires a >= b */
static void bn_sub(bn_t *r, const bn_t *a, const bn_t *b) {
  size_t m = a->n;
  uint32_t *buf = (uint32_t *)malloc((m + 1) * sizeof(uint32_t));
  int64_t br = 0;
  for (size_t i = 0; i < m; i++) {
    int64_t t = (int64_t)a->d[i] - (int64_t)(i < b->n ? b->d[i] : 0) - br;
    if (t < 0) { t += (int64_t)1 << 32; br = 1; } else br = 0;
    buf[i] = (uint32_t)t;
  }
  bn_take(r, buf, m, m + 1);
}
static void bn_mul(bn_t *r, const bn_t *a, const bn_t *b) {
  if (a->n == 0 || b->n == 0) { bn_set(r, 0); return; }
  size_t m = a->n + b->n;
  uint32_t *buf = (uint32_t *)calloc(m + 1, sizeof(uint32_t));
  for (size_t i = 0; i < a->n; i++) {
    uint64_t c = 0, ai = a->d[i];
    for (size_t j = 0; j < b->n; j++) {
      uint64_t t = ai * b->d[j] + buf[i + j] + c;
      buf[i + j] = (uint32_t)t; c = t >> 32;
    }
    buf[i + b->n] = (uint32_t)c;
  }
  bn_take(r, buf, m, m + 1);
}
static void bn_shl_limbs(bn_t *r, const bn_t *a, size_t k) {
  uint32_t *buf = (uint32_t *)calloc(a->n + k + 1, sizeof(uint32_t));
  if (a->n) memcpy(buf + k, a->d, a->n * sizeof(uint32_t));
  bn_take(r, buf, a->n + k, a->n + k + 1);
}
static void bn_shr_bits(bn_t *r, const bn_t *a, size_t s) {
  size_t qn = s / 32, b = s % 32;
  if (qn >= a->n) { bn_set(r, 0); return; }
  size_t m = a->n - qn;
  uint32_t *buf = (uint32_t *)malloc((m + 1) * sizeof(uint32_t));
  for (size_t i = 0; i < m; i++) {
    uint32_t lo = a->d[i + qn] >> b;
    uint32_t hi = (b && i + qn + 1 < a->n) ? (a->d[i + qn + 1] << (32 - b)) : 0;
    buf[i] = lo | hi;
  }
  bn_take(r, buf, m, m + 1);
}
static uint32_t bn_divsmall(bn_t *a, uint32_t v) {
  uint64_t rem = 0;
  for (size_t i = a->n; i-- > 0;) {
    uint64_t cur = (rem << 32) | a->d[i];
    a->d[i] = (uint32_t)(cur / v); rem = cur % v;
  }
  bn_trim(a);
  return (uint32_t)rem;
}
static size_t bn_bitlen(const bn_t *a) {
  if (a->n == 0) return 0;
  size_t bits = (a->n - 1) * 32;
  uint32_t top = a->d[a->n - 1];
  while (top) { bits++; top >>= 1; }
  return bits;
}

static bn_t ONE = {NULL, 0, 0};

/* out = ceiling(N / D), D > 0 */
static void bn_ceil_div(bn_t *out, const bn_t *N, const bn_t *D) {
  size_t nb = bn_bitlen(N), db = bn_bitlen(D);
  if (nb < db) { bn_set(out, N->n ? 1 : 0); return; }
  size_t qbits = nb - db + 1;
  bn_t R = {NULL, 0, 0};
  bn_shr_bits(&R, N, qbits);
  bn_reserve(&R, D->n + 2);
  uint32_t *qb = (uint32_t *)calloc(qbits / 32 + 2, sizeof(uint32_t));
  for (size_t i = qbits; i-- > 0;) {
    uint32_t c = (N->d[i / 32] >> (i % 32)) & 1u;
    for (size_t j = 0; j < R.n; j++) {
      uint32_t v = R.d[j];
      R.d[j] = (v << 1) | c; c = v >> 31;
    }
    if (c) R.d[R.n++] = c;
    if (bn_cmp(&R, D) >= 0) {
      int64_t br = 0;
      for (size_t j = 0; j < R.n; j++) {
        int64_t t = (int64_t)R.d[j] - (int64_t)(j < D->n ? D->d[j] : 0) - br;
        if (t < 0) { t += (int64_t)1 << 32; br = 1; } else br = 0;
        R.d[j] = (uint32_t)t;
      }
      bn_trim(&R);
      qb[i / 32] |= 1u << (i % 32);
    }
  }
  bn_take(out, qb, qbits / 32 + 1, qbits / 32 + 2);
  if (R.n) bn_add(out, out, &ONE);
  bn_free(&R);
}

static bn_t S = {NULL, 0, 0}, Qd = {NULL, 0, 0};
static bn_t Xlo = {NULL, 0, 0}, Xhi = {NULL, 0, 0};
static size_t prec = 0;
static bn_t *terms = NULL;
static size_t nterms = 0, tcap = 0;
static int inited = 0;

/* Xlo <= Pi * 2^(32*F) <= Xhi */
static void compute_pi(size_t F) {
  size_t W = F + 2;
  bn_t pos = {NULL, 0, 0}, neg = {NULL, 0, 0}, term = {NULL, 0, 0}, t = {NULL, 0, 0};
  static const uint32_t xs[2] = {5, 239}, ms[2] = {16, 4};
  for (int p = 0; p < 2; p++) {
    bn_reserve(&term, W + 1);
    memset(term.d, 0, (W + 1) * sizeof(uint32_t));
    term.d[W] = ms[p]; term.n = W + 1;
    bn_divsmall(&term, xs[p]);
    uint32_t xx = xs[p] * xs[p];
    for (uint32_t k = 0; term.n > 0; k++) {
      bn_copy(&t, &term);
      bn_divsmall(&t, 2 * k + 1);
      int positive = ((k % 2 == 0) == (p == 0));
      if (positive) bn_add(&pos, &pos, &t); else bn_add(&neg, &neg, &t);
      bn_divsmall(&term, xx);
    }
  }
  bn_sub(&pos, &pos, &neg);
  bn_shr_bits(&pos, &pos, 64);
  bn_sub(&Xlo, &pos, &ONE);
  bn_add(&Xhi, &pos, &ONE);
  bn_add(&Xhi, &Xhi, &ONE);
  bn_free(&pos); bn_free(&neg); bn_free(&term); bn_free(&t);
}

/* out = ceiling(X*Q / (Q*2^(32*prec) - X*S)) ; 0 if denominator not positive */
static int eval_term(bn_t *out, const bn_t *X) {
  bn_t N = {NULL, 0, 0}, T = {NULL, 0, 0}, M = {NULL, 0, 0}, D = {NULL, 0, 0};
  bn_mul(&N, X, &Qd);
  bn_shl_limbs(&T, &Qd, prec);
  bn_mul(&M, X, &S);
  int ok = bn_cmp(&T, &M) > 0;
  if (ok) {
    bn_sub(&D, &T, &M);
    bn_ceil_div(out, &N, &D);
  }
  bn_free(&N); bn_free(&T); bn_free(&M); bn_free(&D);
  return ok;
}

static void step(void) {
  bn_t lo = {NULL, 0, 0}, hi = {NULL, 0, 0};
  size_t want = 3 * Qd.n + 4;
  if (prec < want) { prec = want; compute_pi(prec); }
  for (;;) {
    int ok = eval_term(&lo, &Xlo) && eval_term(&hi, &Xhi);
    if (ok && bn_cmp(&lo, &hi) == 0) break;
    prec *= 2;
    compute_pi(prec);
  }
  bn_free(&hi);
  if (nterms == tcap) {
    tcap = tcap ? 2 * tcap : 16;
    terms = (bn_t *)realloc(terms, tcap * sizeof(bn_t));
  }
  terms[nterms++] = lo;
  bn_t tmp = {NULL, 0, 0};
  bn_mul(&tmp, &S, &terms[nterms - 1]);
  bn_add(&S, &tmp, &Qd);
  bn_mul(&Qd, &Qd, &terms[nterms - 1]);
  bn_free(&tmp);
}

seq_t a(idx_t n) {
  if (!inited) {
    bn_set(&ONE, 1);
    bn_set(&S, 0);
    bn_set(&Qd, 1);
    inited = 1;
  }
  if (n < 1) return (seq_t)0;
  while ((idx_t)nterms < n) step();
  const bn_t *t = &terms[n - 1];
  seq_t x = 0;
  for (size_t i = t->n; i-- > 0;) {
    x = x * 65536 + (seq_t)(int)(t->d[i] >> 16);
    x = x * 65536 + (seq_t)(int)(t->d[i] & 0xFFFFu);
  }
  return x;
}
