/*
 * A002668  Continued cotangent for e.
 * https://oeis.org/A002668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Lehmer's continued cotangent of e: x(1) = e, a(n) = floor(x(n)),
 *            x(n+1) = (a(n)*x(n) + 1)/(x(n) - a(n)). e is bracketed by E/D <
 *            e < (E+1)/D with D = m!, E = Sum_{j=0..m} m!/j!; the recurrence
 *            is run exactly on both rational endpoints, and a term is
 *            accepted only when both agree (m is doubled unt...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t n; } a2668_bn;

static a2668_bn a2668_alloc(size_t cap) {
  a2668_bn r;
  r.d = (uint32_t *)calloc(cap + 1, sizeof(uint32_t));
  r.n = 0;
  return r;
}
static void a2668_trim(a2668_bn *a) { while (a->n && a->d[a->n - 1] == 0) a->n--; }
static void a2668_free(a2668_bn *a) { free(a->d); a->d = NULL; a->n = 0; }
static a2668_bn a2668_small(uint32_t v) {
  a2668_bn r = a2668_alloc(1);
  r.d[0] = v; r.n = v ? 1 : 0;
  return r;
}
static a2668_bn a2668_copy(const a2668_bn *a) {
  a2668_bn r = a2668_alloc(a->n);
  if (a->n) memcpy(r.d, a->d, a->n * sizeof(uint32_t));
  r.n = a->n;
  return r;
}
static int a2668_cmp(const a2668_bn *a, const a2668_bn *b) {
  size_t i;
  if (a->n != b->n) return a->n < b->n ? -1 : 1;
  for (i = a->n; i-- > 0;)
    if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
  return 0;
}
static a2668_bn a2668_add(const a2668_bn *a, const a2668_bn *b) {
  size_t L = (a->n > b->n ? a->n : b->n) + 1, i;
  a2668_bn r = a2668_alloc(L);
  uint64_t c = 0;
  for (i = 0; i < L; i++) {
    uint64_t s = c + (i < a->n ? a->d[i] : 0) + (i < b->n ? b->d[i] : 0);
    r.d[i] = (uint32_t)s; c = s >> 32;
  }
  r.n = L; a2668_trim(&r);
  return r;
}
static a2668_bn a2668_mul(const a2668_bn *a, const a2668_bn *b) {
  size_t i, j;
  a2668_bn r;
  if (!a->n || !b->n) return a2668_small(0);
  r = a2668_alloc(a->n + b->n);
  for (i = 0; i < a->n; i++) {
    uint64_t c = 0;
    for (j = 0; j < b->n; j++) {
      uint64_t t = (uint64_t)a->d[i] * b->d[j] + r.d[i + j] + c;
      r.d[i + j] = (uint32_t)t; c = t >> 32;
    }
    r.d[i + b->n] = (uint32_t)c;
  }
  r.n = a->n + b->n; a2668_trim(&r);
  return r;
}
static void a2668_divmod(const a2668_bn *a, const a2668_bn *b, a2668_bn *q, a2668_bn *r) {
  size_t i, k, nb = a->n * 32;
  *q = a2668_alloc(a->n); q->n = a->n;
  *r = a2668_alloc(b->n + 1);
  for (i = nb; i-- > 0;) {
    uint32_t carry = (a->d[i / 32] >> (i % 32)) & 1u;
    for (k = 0; k < r->n; k++) {
      uint32_t t = r->d[k];
      r->d[k] = (t << 1) | carry; carry = t >> 31;
    }
    if (carry) r->d[r->n++] = carry;
    if (a2668_cmp(r, b) >= 0) {
      int64_t br = 0;
      for (k = 0; k < r->n; k++) {
        int64_t s = (int64_t)r->d[k] - (k < b->n ? (int64_t)b->d[k] : 0) - br;
        if (s < 0) { s += ((int64_t)1 << 32); br = 1; } else br = 0;
        r->d[k] = (uint32_t)s;
      }
      a2668_trim(r);
      q->d[i / 32] |= 1u << (i % 32);
    }
  }
  a2668_trim(q);
}

static a2668_bn *a2668_terms = NULL;
static size_t a2668_cnt = 0;

static void a2668_run(uint32_t m) {
  a2668_bn D = a2668_small(1), E = a2668_small(1), one = a2668_small(1);
  a2668_bn N1, M1, N2, M2;
  size_t k = 0;
  uint32_t j;
  for (j = m; j >= 1; j--) {
    a2668_bn s = a2668_small(j), t = a2668_mul(&D, &s), u;
    a2668_free(&D); a2668_free(&s); D = t;
    u = a2668_add(&E, &D); a2668_free(&E); E = u;
  }
  N1 = a2668_copy(&E); M1 = a2668_copy(&D);
  N2 = a2668_add(&E, &one); M2 = a2668_copy(&D);
  for (;;) {
    a2668_bn q1, r1, q2, r2, t, nN1, nN2;
    int stop;
    a2668_divmod(&N1, &M1, &q1, &r1);
    a2668_divmod(&N2, &M2, &q2, &r2);
    if (a2668_cmp(&q1, &q2) != 0) {
      a2668_free(&q1); a2668_free(&r1); a2668_free(&q2); a2668_free(&r2);
      break;
    }
    if (k >= a2668_cnt) {
      a2668_terms = (a2668_bn *)realloc(a2668_terms, (k + 1) * sizeof(a2668_bn));
      a2668_terms[k] = a2668_copy(&q1);
      a2668_cnt = k + 1;
    }
    k++;
    stop = (r1.n == 0 || r2.n == 0);
    if (!stop) {
      t = a2668_mul(&q1, &N1); nN1 = a2668_add(&t, &M1); a2668_free(&t);
      t = a2668_mul(&q2, &N2); nN2 = a2668_add(&t, &M2); a2668_free(&t);
      a2668_free(&N1); a2668_free(&M1); a2668_free(&N2); a2668_free(&M2);
      N1 = nN1; M1 = r1; N2 = nN2; M2 = r2;
    } else {
      a2668_free(&r1); a2668_free(&r2);
    }
    a2668_free(&q1); a2668_free(&q2);
    if (stop) break;
  }
  a2668_free(&N1); a2668_free(&M1); a2668_free(&N2); a2668_free(&M2);
  a2668_free(&D); a2668_free(&E); a2668_free(&one);
}

seq_t a(idx_t n) {
  static uint32_t m = 16;
  const a2668_bn *t;
  seq_t v = 0;
  size_t i;
  if (n < 1) return 0;
  while ((idx_t)a2668_cnt < n) {
    a2668_run(m);
    m *= 2;
  }
  t = &a2668_terms[n - 1];
  for (i = t->n; i-- > 0;) {
    int sh;
    for (sh = 24; sh >= 0; sh -= 8) {
      v = v * (seq_t)256 + (seq_t)((t->d[i] >> sh) & 0xFFu);
    }
  }
  return v;
}
