/*
 * A001163  Stirling's formula: numerators of asymptotic series for Gamma
 *          function.
 * https://oeis.org/A001163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, nice
 * Formula:   c_0 = 1, c_n = (1/n) * Sum_{k=0..n-1} B_{n-k+1}*c_k/(n-k+1) for
 *            n > 0; a(n) = numerator(c_n). Only odd n-k contribute, and
 *            B_{2j}/(2j) = (-1)^(j-1)*A000182(j)/(4^j*(4^j-1)), so c_n =
 *            (1/n) * Sum_{j=1..floor((n+1)/2)}
 *            (-1)^(j-1)*A000182(j)/(4^j*(4^j-1)) * c_{n-2j+1}. Exact rational
 *            arithmetic on i...
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 10, i128 n <= 17,
 *            big any n
 * Helpers:   bz_alloc, bz_madd, bz_mcmp, bz_mdivmod, bz_msub, bz_norm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/bz_alloc.h"
#include "../utils/bz_madd.h"
#include "../utils/bz_mcmp.h"
#include "../utils/bz_mdivmod.h"
#include "../utils/bz_msub.h"
#include "../utils/bz_norm.h"

typedef struct { int neg; size_t n; uint32_t *d; } bz;
typedef struct { bz p, q; } rq;

static bz bz_from(uint64_t v, int neg) {
    bz r;
    r.d = bz_alloc(2);
    r.d[0] = (uint32_t)v;
    r.d[1] = (uint32_t)(v >> 32);
    r.n = bz_norm(r.d, 2);
    r.neg = r.n ? neg : 0;
    return r;
}

static bz bz_pow2(idx_t e) {
    bz r;
    size_t L = (size_t)(e / 32) + 1;
    r.d = bz_alloc(L);
    r.d[e / 32] = (uint32_t)1 << (e % 32);
    r.n = L;
    r.neg = 0;
    return r;
}

static void bz_free(bz *x) {
    free(x->d);
    x->d = NULL;
    x->n = 0;
    x->neg = 0;
}

static bz bz_copy(const bz *x) {
    bz r;
    r.d = bz_alloc(x->n);
    if (x->n) memcpy(r.d, x->d, x->n * sizeof(uint32_t));
    r.n = x->n;
    r.neg = x->neg;
    return r;
}

static bz bz_add(const bz *a, const bz *b) {
    bz r;
    size_t m = (a->n > b->n ? a->n : b->n) + 1;
    r.d = bz_alloc(m);
    if (a->neg == b->neg) {
        r.n = bz_madd(r.d, a->d, a->n, b->d, b->n);
        r.neg = a->neg;
    } else if (bz_mcmp(a->d, a->n, b->d, b->n) >= 0) {
        r.n = bz_msub(r.d, a->d, a->n, b->d, b->n);
        r.neg = a->neg;
    } else {
        r.n = bz_msub(r.d, b->d, b->n, a->d, a->n);
        r.neg = b->neg;
    }
    if (!r.n) r.neg = 0;
    return r;
}

static bz bz_mul(const bz *a, const bz *b) {
    bz r;
    size_t i, j;
    if (!a->n || !b->n) return bz_from(0, 0);
    r.d = bz_alloc(a->n + b->n);
    for (i = 0; i < a->n; i++) {
        uint64_t c = 0;
        for (j = 0; j < b->n; j++) {
            c += (uint64_t)a->d[i] * b->d[j] + r.d[i + j];
            r.d[i + j] = (uint32_t)c;
            c >>= 32;
        }
        r.d[i + b->n] = (uint32_t)c;
    }
    r.n = bz_norm(r.d, a->n + b->n);
    r.neg = a->neg ^ b->neg;
    return r;
}

static bz bz_muls(const bz *a, uint32_t m) {
    bz r;
    uint64_t c = 0;
    size_t i;
    r.d = bz_alloc(a->n + 1);
    for (i = 0; i < a->n; i++) { c += (uint64_t)a->d[i] * m; r.d[i] = (uint32_t)c; c >>= 32; }
    r.d[a->n] = (uint32_t)c;
    r.n = bz_norm(r.d, a->n + 1);
    r.neg = r.n ? a->neg : 0;
    return r;
}

/* magnitude division */
static void bz_divmod(const bz *a, const bz *b, bz *q, bz *r) {
    if (bz_mcmp(a->d, a->n, b->d, b->n) < 0) {
        *q = bz_from(0, 0);
        *r = bz_copy(a);
        r->neg = 0;
        return;
    }
    q->d = bz_alloc(a->n - b->n + 1);
    r->d = bz_alloc(b->n);
    bz_mdivmod(a->d, a->n, b->d, b->n, q->d, r->d);
    q->n = bz_norm(q->d, a->n - b->n + 1);
    r->n = bz_norm(r->d, b->n);
    q->neg = 0;
    r->neg = 0;
}

static bz bz_gcd(const bz *a, const bz *b) {
    bz x = bz_copy(a), y = bz_copy(b);
    x.neg = 0;
    y.neg = 0;
    while (y.n) {
        bz q, r;
        bz_divmod(&x, &y, &q, &r);
        bz_free(&q);
        bz_free(&x);
        x = y;
        y = r;
    }
    bz_free(&y);
    return x;
}

static bz bz_divexact(const bz *a, const bz *g) {
    bz q, r;
    bz_divmod(a, g, &q, &r);
    bz_free(&r);
    q.neg = q.n ? a->neg : 0;
    return q;
}

static void rq_reduce(rq *x) {
    bz g;
    if (!x->p.n) {
        bz_free(&x->q);
        x->q = bz_from(1, 0);
        return;
    }
    g = bz_gcd(&x->p, &x->q);
    if (!(g.n == 1 && g.d[0] == 1)) {
        bz p = bz_divexact(&x->p, &g), q = bz_divexact(&x->q, &g);
        bz_free(&x->p);
        bz_free(&x->q);
        x->p = p;
        x->q = q;
    }
    bz_free(&g);
}

static rq *t_v = NULL;   /* t_v[j] = B_{2j}/(2j), j = 1..t_n */
static idx_t t_n = 0;
static rq *c_v = NULL;   /* c_v[i] = c_i, i < c_n */
static idx_t c_n = 0, c_cap = 0;

static void t_ensure(idx_t K) {
    idx_t K2, k, j;
    bz *T;
    if (K <= t_n) return;
    K2 = K < 2 * t_n ? 2 * t_n : K;
    if (K2 < 8) K2 = 8;
    /* tangent numbers A000182 by the Brent-Harvey recurrence */
    T = (bz *)malloc((size_t)(K2 + 1) * sizeof(bz));
    T[0] = bz_from(0, 0);
    T[1] = bz_from(1, 0);
    for (k = 2; k <= K2; k++) T[k] = bz_muls(&T[k - 1], (uint32_t)(k - 1));
    for (k = 2; k <= K2; k++) {
        for (j = k; j <= K2; j++) {
            bz x = bz_muls(&T[j - 1], (uint32_t)(j - k));
            bz y = bz_muls(&T[j], (uint32_t)(j - k + 2));
            bz_free(&T[j]);
            T[j] = bz_add(&x, &y);
            bz_free(&x);
            bz_free(&y);
        }
    }
    t_v = (rq *)realloc(t_v, (size_t)(K2 + 1) * sizeof(rq));
    for (k = t_n + 1; k <= K2; k++) {
        bz p4 = bz_pow2(2 * k), m1 = bz_from(1, 1), q1 = bz_add(&p4, &m1);
        t_v[k].p = bz_copy(&T[k]);
        t_v[k].p.neg = (k % 2 == 0) && t_v[k].p.n;
        t_v[k].q = bz_mul(&p4, &q1);
        bz_free(&p4);
        bz_free(&m1);
        bz_free(&q1);
        rq_reduce(&t_v[k]);
    }
    for (k = 0; k <= K2; k++) bz_free(&T[k]);
    free(T);
    t_n = K2;
}

static void c_push(rq x) {
    if (c_n == c_cap) {
        c_cap = c_cap ? 2 * c_cap : 16;
        c_v = (rq *)realloc(c_v, (size_t)c_cap * sizeof(rq));
    }
    c_v[c_n++] = x;
}

static void c_extend(void) {
    idx_t i = c_n, j;
    rq S;
    bz nn, nq;
    S.p = bz_from(0, 0);
    S.q = bz_from(1, 0);
    t_ensure((i + 1) / 2);
    for (j = 1; 2 * j - 1 <= i; j++) {
        const rq *c = &c_v[i - (2 * j - 1)];
        bz num, den, g, dg, sg, x, y, np, sq;
        if (!c->p.n) continue;
        num = bz_mul(&t_v[j].p, &c->p);
        den = bz_mul(&t_v[j].q, &c->q);
        g = bz_gcd(&S.q, &den);
        dg = bz_divexact(&den, &g);
        sg = bz_divexact(&S.q, &g);
        x = bz_mul(&S.p, &dg);
        y = bz_mul(&num, &sg);
        np = bz_add(&x, &y);
        sq = bz_mul(&S.q, &dg);
        bz_free(&num); bz_free(&den); bz_free(&g); bz_free(&dg);
        bz_free(&sg); bz_free(&x); bz_free(&y);
        bz_free(&S.p); bz_free(&S.q);
        S.p = np;
        S.q = sq;
    }
    nn = bz_from((uint64_t)i, 0);
    nq = bz_mul(&S.q, &nn);
    bz_free(&nn);
    bz_free(&S.q);
    S.q = nq;
    rq_reduce(&S);
    c_push(S);
}

static seq_t bz_to_seq(const bz *x) {
    seq_t r = 0;
    size_t i = x->n;
    while (i-- > 0) {
        r = r * 65536 + (int)(x->d[i] >> 16);
        r = r * 65536 + (int)(x->d[i] & 0xFFFFu);
    }
    if (x->neg) r = -r;
    return r;
}

seq_t a(idx_t n) {
    if (c_n == 0) {
        rq one;
        one.p = bz_from(1, 0);
        one.q = bz_from(1, 0);
        c_push(one);
    }
    while (c_n <= n) c_extend();
    return bz_to_seq(&c_v[n].p);
}
