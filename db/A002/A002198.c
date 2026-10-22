/*
 * A002198  Denominators of coefficients for numerical integration.
 * https://oeis.org/A002198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator of the coefficient of d^(2n+2) in
 *            d/(2*arcsinh(d/2)) (the midpoint-rule central-difference
 *            integration series; equivalent to the Bernoulli / central
 *            factorial formula). With u = x^2, x = d/2: w = 1/g, g(u) =
 *            Sum_{j>=0} (-1)^j*C(2j,j)/(2j+1)*u^j, so w_0 = 1, w_m =
 *            -Sum_{j=1..m}...
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   a2198_cmpmag
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a2198_cmpmag.h"

typedef struct { uint32_t *d; size_t n; int neg; } a2198_big;

static a2198_big *a2198_Wt = NULL;
static a2198_big *a2198_Ct = NULL;
static unsigned char *a2198_comp = NULL;
static long long a2198_M = -1;

static void a2198_norm(a2198_big *x) {
    while (x->n && x->d[x->n - 1] == 0) x->n--;
    if (!x->n) x->neg = 0;
}

static void a2198_mul_small(a2198_big *x, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < x->n; i++) {
        uint64_t t = (uint64_t)x->d[i] * m + c;
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) {
        x->d = (uint32_t *)realloc(x->d, (x->n + 1) * sizeof(uint32_t));
        x->d[x->n++] = (uint32_t)c;
    }
}

static uint32_t a2198_div_small(a2198_big *x, uint32_t dv) {
    uint64_t r = 0;
    size_t i = x->n;
    while (i-- > 0) {
        uint64_t t = (r << 32) | x->d[i];
        x->d[i] = (uint32_t)(t / dv);
        r = t % dv;
    }
    a2198_norm(x);
    return (uint32_t)r;
}

static uint32_t a2198_mod_small(const a2198_big *x, uint32_t dv) {
    uint64_t r = 0;
    size_t i = x->n;
    while (i-- > 0) {
        uint64_t t = (r << 32) | x->d[i];
        r = t % dv;
    }
    return (uint32_t)r;
}

static a2198_big a2198_copy(const a2198_big *x) {
    a2198_big r;
    r.n = x->n;
    r.neg = x->neg;
    r.d = NULL;
    if (r.n) {
        r.d = (uint32_t *)malloc(r.n * sizeof(uint32_t));
        memcpy(r.d, x->d, r.n * sizeof(uint32_t));
    }
    return r;
}

static a2198_big a2198_mul(const a2198_big *a, const a2198_big *b) {
    a2198_big r;
    size_t i, j;
    r.d = NULL;
    r.n = 0;
    r.neg = 0;
    if (!a->n || !b->n) return r;
    r.n = a->n + b->n;
    r.neg = a->neg ^ b->neg;
    r.d = (uint32_t *)calloc(r.n, sizeof(uint32_t));
    for (i = 0; i < a->n; i++) {
        uint64_t c = 0, ai = a->d[i];
        for (j = 0; j < b->n; j++) {
            uint64_t t = ai * b->d[j] + r.d[i + j] + c;
            r.d[i + j] = (uint32_t)t;
            c = t >> 32;
        }
        r.d[i + b->n] = (uint32_t)c;
    }
    a2198_norm(&r);
    return r;
}

static void a2198_addto(a2198_big *acc, const a2198_big *t, int tneg) {
    size_t n, i;
    int c;
    if (!t->n) return;
    if (acc->n && acc->neg != tneg) c = a2198_cmpmag(acc->d, acc->n, t->d, t->n);
    else c = 2;
    n = (acc->n > t->n ? acc->n : t->n) + 1;
    acc->d = (uint32_t *)realloc(acc->d, n * sizeof(uint32_t));
    for (i = acc->n; i < n; i++) acc->d[i] = 0;
    if (c == 2) {
        uint64_t carry = 0;
        for (i = 0; i < n; i++) {
            uint64_t s = (uint64_t)acc->d[i] + (i < t->n ? t->d[i] : 0) + carry;
            acc->d[i] = (uint32_t)s;
            carry = s >> 32;
        }
        acc->neg = tneg;
    } else if (c >= 0) {
        int64_t borrow = 0;
        for (i = 0; i < n; i++) {
            int64_t s = (int64_t)acc->d[i] - (int64_t)(i < t->n ? t->d[i] : 0) - borrow;
            if (s < 0) { s += 4294967296LL; borrow = 1; } else borrow = 0;
            acc->d[i] = (uint32_t)s;
        }
    } else {
        int64_t borrow = 0;
        for (i = 0; i < n; i++) {
            int64_t s = (int64_t)(i < t->n ? t->d[i] : 0) - (int64_t)acc->d[i] - borrow;
            if (s < 0) { s += 4294967296LL; borrow = 1; } else borrow = 0;
            acc->d[i] = (uint32_t)s;
        }
        acc->neg = tneg;
    }
    acc->n = n;
    a2198_norm(acc);
}

static void a2198_grow(long long M2) {
    long long M0, lim, i, p, k, m, j;
    uint32_t *ch = NULL;
    size_t nch = 0, cap = 0, q;
    uint64_t cur = 1;
    if (a2198_M < 0) {
        a2198_Wt = (a2198_big *)malloc(sizeof(a2198_big));
        a2198_Ct = (a2198_big *)malloc(sizeof(a2198_big));
        a2198_Wt[0].d = (uint32_t *)malloc(sizeof(uint32_t));
        a2198_Wt[0].d[0] = 1;
        a2198_Wt[0].n = 1;
        a2198_Wt[0].neg = 0;
        a2198_Ct[0].d = (uint32_t *)malloc(sizeof(uint32_t));
        a2198_Ct[0].d[0] = 1;
        a2198_Ct[0].n = 1;
        a2198_Ct[0].neg = 0;
        a2198_M = 0;
    }
    M0 = a2198_M;
    if (M2 <= M0) return;
    lim = 2 * M2 + 1;
    free(a2198_comp);
    a2198_comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    for (i = 3; i * i <= lim; i += 2)
        if (!a2198_comp[i])
            for (k = i * i; k <= lim; k += 2 * i) a2198_comp[k] = 1;

    /* rescale factor F_{M2}/F_{M0} as 32-bit chunks */
    for (p = 3; p <= lim; p += 2) {
        long long d;
        if (a2198_comp[p]) continue;
        d = 2 * M2 / (p - 1) - 2 * M0 / (p - 1);
        while (d-- > 0) {
            if (cur * (uint64_t)p > 0xFFFFFFFFULL) {
                if (nch == cap) {
                    cap = cap ? 2 * cap : 16;
                    ch = (uint32_t *)realloc(ch, cap * sizeof(uint32_t));
                }
                ch[nch++] = (uint32_t)cur;
                cur = 1;
            }
            cur *= (uint64_t)p;
        }
    }
    if (cur > 1) {
        if (nch == cap) {
            cap = cap ? 2 * cap : 16;
            ch = (uint32_t *)realloc(ch, cap * sizeof(uint32_t));
        }
        ch[nch++] = (uint32_t)cur;
    }
    for (k = 0; k <= M0; k++)
        for (q = 0; q < nch; q++) a2198_mul_small(&a2198_Wt[k], ch[q]);
    free(ch);

    a2198_Wt = (a2198_big *)realloc(a2198_Wt, (size_t)(M2 + 1) * sizeof(a2198_big));
    a2198_Ct = (a2198_big *)realloc(a2198_Ct, (size_t)(M2 + 1) * sizeof(a2198_big));
    for (m = M0 + 1; m <= M2; m++) {
        a2198_big *c = &a2198_Ct[m];
        a2198_big *w = &a2198_Wt[m];
        *c = a2198_copy(&a2198_Ct[m - 1]);
        a2198_mul_small(c, (uint32_t)(2 * (2 * m - 1)));
        a2198_div_small(c, (uint32_t)m);
        w->d = NULL;
        w->n = 0;
        w->neg = 0;
        for (j = 1; j <= m; j++) {
            a2198_big t = a2198_mul(&a2198_Wt[m - j], &a2198_Ct[j]);
            a2198_div_small(&t, (uint32_t)(2 * j + 1));
            a2198_addto(w, &t, t.neg ^ ((j & 1) ? 0 : 1));
            free(t.d);
        }
    }
    a2198_M = M2;
}

seq_t a(idx_t n) {
    long long m = (long long)n + 1, M, p, k, v2 = 0, e2;
    const a2198_big *w;
    seq_t r = 1;
    size_t i = 0;
    if (m > a2198_M) a2198_grow(m);
    M = a2198_M;
    w = &a2198_Wt[m];

    while (i < w->n && w->d[i] == 0) { v2 += 32; i++; }
    if (i < w->n) {
        uint32_t x = w->d[i];
        while (!(x & 1)) { x >>= 1; v2++; }
    }
    e2 = 4 * m - (v2 < 4 * m ? v2 : 4 * m);
    for (k = 0; k < e2; k++) r *= 2;

    for (p = 3; p <= 2 * M + 1; p += 2) {
        long long e, v = 0;
        idx_t pp = (idx_t)p;
        if (a2198_comp[p]) continue;
        e = 2 * M / (p - 1);
        if (a2198_mod_small(w, (uint32_t)p) == 0) {
            a2198_big cp = a2198_copy(w);
            while (v < e && a2198_mod_small(&cp, (uint32_t)p) == 0) {
                a2198_div_small(&cp, (uint32_t)p);
                v++;
            }
            free(cp.d);
        }
        for (k = v; k < e; k++) r *= pp;
    }
    return r;
}
