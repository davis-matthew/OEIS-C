/*
 * A002937  An exotic continued fraction (for the real root of x^3 - 8*x -
 *          10).
 * https://oeis.org/A002937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, nonn
 * Formula:   a(n) = n-th partial quotient of the continued fraction of the
 *            real root of x^3 - 8*x - 10. Computed exactly with Lagrange's
 *            method: a(n) = floor of the unique real root (> 1) of P_n, where
 *            P_0(x) = x^3 - 8*x - 10 and P_{n+1}(y) = -y^3 * P_n(a(n) + 1/y).
 *            The coefficients are big integers
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 9591, u64 n <= 9591,
 *            u128 n <= 9591, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t n, cap; int neg; } a2937_bz;

static void a2937_reserve(a2937_bz *x, size_t c) {
    if (c > x->cap) {
        size_t nc = c * 2 + 8;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}

static void a2937_trim(a2937_bz *x) {
    while (x->n && x->d[x->n - 1] == 0) x->n--;
    if (!x->n) x->neg = 0;
}

static void a2937_set(a2937_bz *x, long long v) {
    unsigned long long m = v < 0 ? 0ULL - (unsigned long long)v : (unsigned long long)v;
    a2937_reserve(x, 2);
    x->n = 0;
    x->neg = v < 0;
    while (m) { x->d[x->n++] = (uint32_t)m; m >>= 32; }
    a2937_trim(x);
}

static void a2937_copy(a2937_bz *r, const a2937_bz *x) {
    a2937_reserve(r, x->n + 1);
    if (x->n) memcpy(r->d, x->d, x->n * sizeof(uint32_t));
    r->n = x->n;
    r->neg = x->neg;
}

static void a2937_mul32(a2937_bz *x, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < x->n; i++) {
        uint64_t t = (uint64_t)x->d[i] * m + c;
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) { a2937_reserve(x, x->n + 1); x->d[x->n++] = (uint32_t)c; }
    a2937_trim(x);
}

static void a2937_mul64(a2937_bz *x, uint64_t k) {
    uint32_t k0 = (uint32_t)k, k1 = (uint32_t)(k >> 32);
    size_t n = x->n, i;
    uint32_t *o;
    uint64_t c;
    if (k1 == 0) { a2937_mul32(x, k0); return; }
    o = (uint32_t *)calloc(n + 2, sizeof(uint32_t));
    c = 0;
    for (i = 0; i < n; i++) {
        uint64_t t = (uint64_t)x->d[i] * k0 + c;
        o[i] = (uint32_t)t;
        c = t >> 32;
    }
    o[n] = (uint32_t)c;
    c = 0;
    for (i = 0; i < n; i++) {
        uint64_t t = (uint64_t)x->d[i] * k1 + o[i + 1] + c;
        o[i + 1] = (uint32_t)t;
        c = t >> 32;
    }
    o[n + 1] = (uint32_t)c;
    a2937_reserve(x, n + 2);
    memcpy(x->d, o, (n + 2) * sizeof(uint32_t));
    x->n = n + 2;
    free(o);
    a2937_trim(x);
}

static int a2937_cmpmag(const a2937_bz *a, const a2937_bz *b) {
    size_t i;
    if (a->n != b->n) return a->n < b->n ? -1 : 1;
    for (i = a->n; i-- > 0;)
        if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    return 0;
}

/* r += x (signed) */
static void a2937_add(a2937_bz *r, const a2937_bz *x) {
    size_t i;
    if (!x->n) return;
    if (!r->n) { a2937_copy(r, x); return; }
    if (r->neg == x->neg) {
        size_t m = (r->n > x->n ? r->n : x->n) + 1;
        uint64_t c = 0;
        a2937_reserve(r, m);
        for (i = r->n; i < m; i++) r->d[i] = 0;
        for (i = 0; i < m; i++) {
            uint64_t t = (uint64_t)r->d[i] + (i < x->n ? x->d[i] : 0) + c;
            r->d[i] = (uint32_t)t;
            c = t >> 32;
        }
        r->n = m;
        a2937_trim(r);
    } else {
        int cmp = a2937_cmpmag(r, x);
        long long borrow = 0;
        if (cmp == 0) { r->n = 0; r->neg = 0; return; }
        if (cmp > 0) {
            for (i = 0; i < r->n; i++) {
                long long t = (long long)r->d[i] - (long long)(i < x->n ? x->d[i] : 0) - borrow;
                if (t < 0) { t += 4294967296LL; borrow = 1; } else borrow = 0;
                r->d[i] = (uint32_t)t;
            }
            a2937_trim(r);
        } else {
            a2937_reserve(r, x->n);
            for (i = r->n; i < x->n; i++) r->d[i] = 0;
            for (i = 0; i < x->n; i++) {
                long long t = (long long)x->d[i] - (long long)r->d[i] - borrow;
                if (t < 0) { t += 4294967296LL; borrow = 1; } else borrow = 0;
                r->d[i] = (uint32_t)t;
            }
            r->n = x->n;
            r->neg = x->neg;
            a2937_trim(r);
        }
    }
}

static a2937_bz a2937_c[4], a2937_v, a2937_t;
static uint64_t *a2937_terms = NULL;
static idx_t a2937_cnt = 0, a2937_cap = 0;
static int a2937_init = 0;

static int a2937_sign_at(uint64_t k) {
    int j;
    a2937_copy(&a2937_v, &a2937_c[3]);
    for (j = 2; j >= 0; j--) {
        a2937_mul64(&a2937_v, k);
        a2937_add(&a2937_v, &a2937_c[j]);
    }
    if (!a2937_v.n) return 0;
    return a2937_v.neg ? -1 : 1;
}

static uint64_t a2937_step(void) {
    uint64_t lo = 1, hi = 2, a0;
    int i, j;
    a2937_bz tmp;
    while (a2937_sign_at(hi) < 0) { lo = hi; hi *= 2; }
    while (hi - lo > 1) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (a2937_sign_at(mid) < 0) lo = mid; else hi = mid;
    }
    a0 = lo;
    /* Taylor shift: P(x) -> P(x + a0) */
    for (i = 0; i < 3; i++) {
        for (j = 2; j >= i; j--) {
            a2937_copy(&a2937_t, &a2937_c[j + 1]);
            a2937_mul64(&a2937_t, a0);
            a2937_add(&a2937_c[j], &a2937_t);
        }
    }
    /* Reverse and negate: new c[k] = -old c[3-k] */
    tmp = a2937_c[0]; a2937_c[0] = a2937_c[3]; a2937_c[3] = tmp;
    tmp = a2937_c[1]; a2937_c[1] = a2937_c[2]; a2937_c[2] = tmp;
    for (i = 0; i < 4; i++)
        if (a2937_c[i].n) a2937_c[i].neg = !a2937_c[i].neg;
    return a0;
}

seq_t a(idx_t n) {
    if (!a2937_init) {
        int i;
        for (i = 0; i < 4; i++) { a2937_c[i].d = NULL; a2937_c[i].n = 0; a2937_c[i].cap = 0; a2937_c[i].neg = 0; }
        a2937_v.d = NULL; a2937_v.n = 0; a2937_v.cap = 0; a2937_v.neg = 0;
        a2937_t.d = NULL; a2937_t.n = 0; a2937_t.cap = 0; a2937_t.neg = 0;
        a2937_set(&a2937_c[0], -10);
        a2937_set(&a2937_c[1], -8);
        a2937_set(&a2937_c[2], 0);
        a2937_set(&a2937_c[3], 1);
        a2937_init = 1;
    }
    while (a2937_cnt <= n) {
        if (a2937_cnt >= a2937_cap) {
            idx_t nc = a2937_cap ? a2937_cap * 2 : 64;
            a2937_terms = (uint64_t *)realloc(a2937_terms, (size_t)nc * sizeof(uint64_t));
            a2937_cap = nc;
        }
        a2937_terms[a2937_cnt] = a2937_step();
        a2937_cnt++;
    }
    return (seq_t)a2937_terms[n];
}
