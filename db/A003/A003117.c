/*
 * A003117  Continued fraction for fifth root of 3.
 * https://oeis.org/A003117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   continued fraction of the real root of x^5 - 3, found by exact
 *            integer Lagrange/Vincent steps: a(k) = floor of the root of P_k,
 *            and P_{k+1}(y) = y^5 * P_k(a(k) + 1/y), starting from P_0(x) =
 *            x^5 - 3
 * Range:     u8 n <= 41, u16 n <= 9433, u32 n <= 9433, u64 n <= 9433,
 *            u128 n <= 9433, big any n
 * Helpers:   a3117_cmpmag
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a3117_cmpmag.h"

typedef struct { uint32_t *d; size_t n; size_t cap; int neg; } a3117_bn;

static void a3117_ens(a3117_bn *x, size_t c) {
    if (x->cap < c) {
        size_t nc = c * 2 + 8;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}

static void a3117_trim(a3117_bn *x) {
    while (x->n && !x->d[x->n - 1]) x->n--;
    if (!x->n) x->neg = 0;
}

/* r += (bneg ? -1 : 1) * |b| */
static void a3117_add(a3117_bn *r, const uint32_t *b, size_t bl, int bneg) {
    size_t i;
    if (bl == 0) return;
    if (r->n == 0) r->neg = bneg;
    if (r->neg == bneg) {
        size_t L = (r->n > bl ? r->n : bl) + 1;
        uint64_t carry = 0;
        a3117_ens(r, L);
        for (i = r->n; i < L; i++) r->d[i] = 0;
        for (i = 0; i < L; i++) {
            uint64_t s = (uint64_t)r->d[i] + carry + (i < bl ? b[i] : 0);
            r->d[i] = (uint32_t)s;
            carry = s >> 32;
        }
        r->n = L;
    } else if (a3117_cmpmag(r->d, r->n, b, bl) >= 0) {
        int64_t borrow = 0;
        for (i = 0; i < r->n; i++) {
            int64_t s = (int64_t)r->d[i] - borrow - (int64_t)(i < bl ? b[i] : 0);
            borrow = s < 0;
            if (s < 0) s += ((int64_t)1 << 32);
            r->d[i] = (uint32_t)s;
        }
    } else {
        int64_t borrow = 0;
        a3117_ens(r, bl);
        for (i = r->n; i < bl; i++) r->d[i] = 0;
        for (i = 0; i < bl; i++) {
            int64_t s = (int64_t)b[i] - borrow - (int64_t)r->d[i];
            borrow = s < 0;
            if (s < 0) s += ((int64_t)1 << 32);
            r->d[i] = (uint32_t)s;
        }
        r->n = bl;
        r->neg = bneg;
    }
    a3117_trim(r);
}

static void a3117_mulsmall(a3117_bn *x, uint32_t m) {
    uint64_t carry = 0;
    size_t i;
    if (x->n == 0) return;
    a3117_ens(x, x->n + 1);
    for (i = 0; i < x->n; i++) {
        uint64_t p = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)p;
        carry = p >> 32;
    }
    x->d[x->n++] = (uint32_t)carry;
    a3117_trim(x);
}

static void a3117_copy(a3117_bn *dst, const a3117_bn *src) {
    a3117_ens(dst, src->n + 1);
    if (src->n) memcpy(dst->d, src->d, src->n * sizeof(uint32_t));
    dst->n = src->n;
    dst->neg = src->neg;
}

static int a3117_sgn(const a3117_bn *x) {
    return x->n == 0 ? 0 : (x->neg ? -1 : 1);
}

static a3117_bn a3117_c[6];
static a3117_bn a3117_tmp;
static uint64_t *a3117_terms = NULL;
static idx_t a3117_cnt = 0, a3117_cap = 0;

static int a3117_eval_sign(uint32_t m) {
    int j;
    a3117_copy(&a3117_tmp, &a3117_c[5]);
    for (j = 4; j >= 0; j--) {
        a3117_mulsmall(&a3117_tmp, m);
        a3117_add(&a3117_tmp, a3117_c[j].d, a3117_c[j].n, a3117_c[j].neg);
    }
    return a3117_sgn(&a3117_tmp);
}

static void a3117_step(void) {
    int s0 = a3117_sgn(&a3117_c[0]);
    uint64_t lo = 0, hi = 1, aq;
    int i, j;
    while (a3117_eval_sign((uint32_t)hi) == s0) {
        lo = hi;
        hi *= 2;
    }
    while (hi - lo > 1) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (a3117_eval_sign((uint32_t)mid) == s0) lo = mid; else hi = mid;
    }
    aq = lo;
    /* Taylor shift: P(x) -> P(x + aq) */
    if (aq > 0) {
        for (i = 0; i < 5; i++) {
            for (j = 4; j >= i; j--) {
                if (aq == 1) {
                    a3117_add(&a3117_c[j], a3117_c[j + 1].d, a3117_c[j + 1].n, a3117_c[j + 1].neg);
                } else {
                    a3117_copy(&a3117_tmp, &a3117_c[j + 1]);
                    a3117_mulsmall(&a3117_tmp, (uint32_t)aq);
                    a3117_add(&a3117_c[j], a3117_tmp.d, a3117_tmp.n, a3117_tmp.neg);
                }
            }
        }
    }
    /* reverse: y^5 * P(1/y) */
    for (i = 0; i < 3; i++) {
        a3117_bn t = a3117_c[i];
        a3117_c[i] = a3117_c[5 - i];
        a3117_c[5 - i] = t;
    }
    if (a3117_cnt == a3117_cap) {
        idx_t nc = a3117_cap * 2 + 64;
        a3117_terms = (uint64_t *)realloc(a3117_terms, nc * sizeof(uint64_t));
        a3117_cap = nc;
    }
    a3117_terms[a3117_cnt++] = aq;
}

seq_t a(idx_t n) {
    static int init = 0;
    if (!init) {
        int i;
        for (i = 0; i < 6; i++) {
            a3117_c[i].d = NULL; a3117_c[i].n = 0; a3117_c[i].cap = 0; a3117_c[i].neg = 0;
        }
        a3117_tmp.d = NULL; a3117_tmp.n = 0; a3117_tmp.cap = 0; a3117_tmp.neg = 0;
        a3117_ens(&a3117_c[0], 1);
        a3117_c[0].d[0] = 3; a3117_c[0].n = 1; a3117_c[0].neg = 1;
        a3117_ens(&a3117_c[5], 1);
        a3117_c[5].d[0] = 1; a3117_c[5].n = 1; a3117_c[5].neg = 0;
        init = 1;
    }
    while (a3117_cnt <= n) a3117_step();
    return (seq_t)a3117_terms[n];
}
