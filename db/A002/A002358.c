/*
 * A002358  Numerators of convergents to cube root of 5.
 * https://oeis.org/A002358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), a(-1) = 1, a(0) = c(0) = 1, where
 *            c(k) are the continued fraction partial quotients of 5^(1/3).
 *            The c(k) are computed exactly: keep an integer cubic f whose
 *            only real root is the k-th complete quotient (start f(y) = y^3 -
 *            5). Then c(k) = floor(root), found from sign c...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 17, u64 n <= 37, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct { uint32_t *d; size_t n, cap; int neg; } q2358_bz;

static q2358_bz q2358_c[4], q2358_V, q2358_S, q2358_T;
static uint32_t *q2358_cf = NULL;
static size_t q2358_cfn = 0, q2358_cfcap = 0;
static int q2358_init = 0;

static void q2358_reserve(q2358_bz *x, size_t need) {
    if (x->cap < need) {
        size_t c = need * 2 + 4;
        x->d = (uint32_t *)realloc(x->d, c * sizeof(uint32_t));
        x->cap = c;
    }
}

static void q2358_trim(q2358_bz *x) {
    while (x->n && x->d[x->n - 1] == 0) x->n--;
    if (!x->n) x->neg = 0;
}

static void q2358_set(q2358_bz *x, const q2358_bz *y) {
    q2358_reserve(x, y->n);
    if (y->n) memcpy(x->d, y->d, y->n * sizeof(uint32_t));
    x->n = y->n;
    x->neg = y->neg;
}

static void q2358_setsmall(q2358_bz *x, uint32_t v, int neg) {
    q2358_reserve(x, 1);
    x->d[0] = v;
    x->n = v ? 1 : 0;
    x->neg = v ? neg : 0;
}

static void q2358_muls(q2358_bz *x, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < x->n; i++) {
        uint64_t t = (uint64_t)x->d[i] * m + c;
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) {
        q2358_reserve(x, x->n + 1);
        x->d[x->n++] = (uint32_t)c;
    }
    q2358_trim(x);
}

static int q2358_cmpmag(const q2358_bz *x, const q2358_bz *y) {
    size_t i;
    if (x->n != y->n) return x->n < y->n ? -1 : 1;
    for (i = x->n; i-- > 0;) {
        if (x->d[i] != y->d[i]) return x->d[i] < y->d[i] ? -1 : 1;
    }
    return 0;
}

static void q2358_addmag(q2358_bz *x, const q2358_bz *y) {
    size_t m = x->n > y->n ? x->n : y->n, i;
    uint64_t c = 0;
    q2358_reserve(x, m + 1);
    for (i = x->n; i < m; i++) x->d[i] = 0;
    for (i = 0; i < m; i++) {
        uint64_t t = (uint64_t)x->d[i] + (i < y->n ? y->d[i] : 0) + c;
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    x->n = m;
    if (c) x->d[x->n++] = (uint32_t)c;
}

/* |x| -= |y|, requires |x| >= |y| */
static void q2358_submag(q2358_bz *x, const q2358_bz *y) {
    size_t i;
    int64_t b = 0;
    for (i = 0; i < x->n; i++) {
        int64_t t = (int64_t)x->d[i] - (int64_t)(i < y->n ? y->d[i] : 0) - b;
        if (t < 0) { t += ((int64_t)1 << 32); b = 1; } else b = 0;
        x->d[i] = (uint32_t)t;
    }
    q2358_trim(x);
}

static void q2358_add(q2358_bz *x, const q2358_bz *y) {
    if (y->n == 0) return;
    if (x->n == 0) { q2358_set(x, y); return; }
    if (x->neg == y->neg) {
        q2358_addmag(x, y);
    } else {
        int c = q2358_cmpmag(x, y);
        if (c == 0) { x->n = 0; x->neg = 0; return; }
        if (c > 0) {
            q2358_submag(x, y);
        } else {
            q2358_set(&q2358_S, y);
            q2358_submag(&q2358_S, x);
            q2358_set(x, &q2358_S);
        }
    }
    q2358_trim(x);
}

static int q2358_sign_at(uint32_t t) {
    int k;
    q2358_set(&q2358_V, &q2358_c[3]);
    for (k = 2; k >= 0; k--) {
        q2358_muls(&q2358_V, t);
        q2358_add(&q2358_V, &q2358_c[k]);
    }
    if (q2358_V.n == 0) return 0;
    return q2358_V.neg ? -1 : 1;
}

static void q2358_step(void) {
    int s1 = q2358_sign_at(1);
    uint32_t lo = 1, hi, a;
    int i, k;
    q2358_bz tmp;
    while (lo < 0x40000000u && q2358_sign_at(lo * 2) == s1) lo *= 2;
    hi = lo * 2;
    while (hi - lo > 1) {
        uint32_t mid = lo + (hi - lo) / 2;
        if (q2358_sign_at(mid) == s1) lo = mid; else hi = mid;
    }
    a = lo;
    if (q2358_cfn >= q2358_cfcap) {
        q2358_cfcap = q2358_cfcap * 2 + 64;
        q2358_cf = (uint32_t *)realloc(q2358_cf, q2358_cfcap * sizeof(uint32_t));
    }
    q2358_cf[q2358_cfn++] = a;
    /* Taylor shift: f(y) -> f(y + a) */
    for (i = 0; i < 3; i++) {
        for (k = 2; k >= i; k--) {
            q2358_set(&q2358_T, &q2358_c[k + 1]);
            q2358_muls(&q2358_T, a);
            q2358_add(&q2358_c[k], &q2358_T);
        }
    }
    /* reverse coefficients: z^3 f(a + 1/z) */
    tmp = q2358_c[0]; q2358_c[0] = q2358_c[3]; q2358_c[3] = tmp;
    tmp = q2358_c[1]; q2358_c[1] = q2358_c[2]; q2358_c[2] = tmp;
}

seq_t a(idx_t n) {
    seq_t p2, p1;
    idx_t i;
    if (!q2358_init) {
        q2358_setsmall(&q2358_c[3], 1, 0);
        q2358_setsmall(&q2358_c[2], 0, 0);
        q2358_setsmall(&q2358_c[1], 0, 0);
        q2358_setsmall(&q2358_c[0], 5, 1);
        q2358_init = 1;
    }
    while ((idx_t)q2358_cfn <= n) q2358_step();
    p2 = (seq_t)1;
    p1 = (seq_t)q2358_cf[0];
    for (i = 1; i <= n; i++) {
        seq_t p = (seq_t)q2358_cf[i] * p1 + p2;
        p2 = p1;
        p1 = p;
    }
    return p1;
}
