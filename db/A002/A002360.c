/*
 * A002360  Numerators of continued fraction convergents to cube root of 6.
 * https://oeis.org/A002360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), a(-1) = 1, a(0) = c(0) = 1, where
 *            c(k) are the continued fraction partial quotients of 6^(1/3).
 *            They are found exactly by keeping an integer cubic whose only
 *            real root is the current complete quotient (start x^3 - 6),
 *            locating floor of that root by checking signs, the...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 17, u64 n <= 31, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t n, cap; int neg; } cr6_num;

static cr6_num cr6_c[4], cr6_acc, cr6_tmp;
static uint32_t *cr6_q = NULL;
static size_t cr6_len = 0, cr6_qcap = 0;
static int cr6_init = 0;

static void cr6_res(cr6_num *x, size_t c) {
    if (c > x->cap) {
        size_t nc = c * 2 + 4;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}

static void cr6_copy(cr6_num *dst, const cr6_num *src) {
    cr6_res(dst, src->n + 1);
    if (src->n) memcpy(dst->d, src->d, src->n * sizeof(uint32_t));
    dst->n = src->n;
    dst->neg = src->neg;
}

static void cr6_set_small(cr6_num *x, uint32_t v, int neg) {
    cr6_res(x, 2);
    if (v == 0) { x->n = 0; x->neg = 0; return; }
    x->d[0] = v; x->n = 1; x->neg = neg;
}

static void cr6_trim(cr6_num *x) {
    while (x->n > 0 && x->d[x->n - 1] == 0) x->n--;
    if (x->n == 0) x->neg = 0;
}

static void cr6_mulsmall(cr6_num *dst, const cr6_num *src, uint32_t m) {
    size_t i, n = src->n;
    uint64_t carry = 0;
    int neg = src->neg;
    if (m == 0 || n == 0) { dst->n = 0; dst->neg = 0; return; }
    cr6_res(dst, n + 1);
    for (i = 0; i < n; i++) {
        uint64_t t = (uint64_t)src->d[i] * m + carry;
        dst->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) dst->d[n++] = (uint32_t)carry;
    dst->n = n;
    dst->neg = neg;
}

static int cr6_cmpmag(const cr6_num *x, const cr6_num *y) {
    size_t i;
    if (x->n != y->n) return x->n < y->n ? -1 : 1;
    for (i = x->n; i-- > 0;) {
        if (x->d[i] != y->d[i]) return x->d[i] < y->d[i] ? -1 : 1;
    }
    return 0;
}

static void cr6_addmag(cr6_num *dst, const cr6_num *src) {
    size_t i, m = dst->n > src->n ? dst->n : src->n;
    uint64_t carry = 0;
    cr6_res(dst, m + 1);
    for (i = dst->n; i < m; i++) dst->d[i] = 0;
    for (i = 0; i < m; i++) {
        uint64_t t = (uint64_t)dst->d[i] + (i < src->n ? src->d[i] : 0) + carry;
        dst->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    dst->n = m;
    if (carry) dst->d[dst->n++] = (uint32_t)carry;
}

/* |dst| = |dst| - |src|, requires |dst| >= |src| */
static void cr6_submag(cr6_num *dst, const cr6_num *src) {
    size_t i;
    int64_t borrow = 0;
    for (i = 0; i < dst->n; i++) {
        int64_t t = (int64_t)dst->d[i] - (i < src->n ? (int64_t)src->d[i] : 0) - borrow;
        if (t < 0) { t += (int64_t)1 << 32; borrow = 1; } else borrow = 0;
        dst->d[i] = (uint32_t)t;
    }
    cr6_trim(dst);
}

/* |dst| = |src| - |dst|, requires |src| > |dst| */
static void cr6_submag_rev(cr6_num *dst, const cr6_num *src) {
    size_t i;
    int64_t borrow = 0;
    cr6_res(dst, src->n + 1);
    for (i = dst->n; i < src->n; i++) dst->d[i] = 0;
    for (i = 0; i < src->n; i++) {
        int64_t t = (int64_t)src->d[i] - (int64_t)dst->d[i] - borrow;
        if (t < 0) { t += (int64_t)1 << 32; borrow = 1; } else borrow = 0;
        dst->d[i] = (uint32_t)t;
    }
    dst->n = src->n;
    cr6_trim(dst);
}

static void cr6_add(cr6_num *dst, const cr6_num *src) {
    if (src->n == 0) return;
    if (dst->n == 0) { cr6_copy(dst, src); return; }
    if (dst->neg == src->neg) { cr6_addmag(dst, src); return; }
    if (cr6_cmpmag(dst, src) >= 0) {
        int s = dst->neg;
        cr6_submag(dst, src);
        if (dst->n) dst->neg = s;
    } else {
        int s = src->neg;
        cr6_submag_rev(dst, src);
        if (dst->n) dst->neg = s;
    }
}

static int cr6_sign_at(uint32_t k) {
    int j;
    cr6_copy(&cr6_acc, &cr6_c[3]);
    for (j = 2; j >= 0; j--) {
        cr6_mulsmall(&cr6_acc, &cr6_acc, k);
        cr6_add(&cr6_acc, &cr6_c[j]);
    }
    if (cr6_acc.n == 0) return 0;
    return cr6_acc.neg ? -1 : 1;
}

static void cr6_step(void) {
    int s3 = cr6_c[3].neg ? -1 : 1;
    uint32_t lo = 1, hi = 2, a0;
    int i, j;
    cr6_num t;
    while (cr6_sign_at(hi) != s3) { lo = hi; hi *= 2; }
    while (hi - lo > 1) {
        uint32_t mid = lo + (hi - lo) / 2;
        if (cr6_sign_at(mid) != s3) lo = mid; else hi = mid;
    }
    a0 = lo;
    if (cr6_len == cr6_qcap) {
        cr6_qcap = cr6_qcap * 2 + 64;
        cr6_q = (uint32_t *)realloc(cr6_q, cr6_qcap * sizeof(uint32_t));
    }
    cr6_q[cr6_len++] = a0;
    /* x -> x + a0 */
    for (i = 0; i < 3; i++) {
        for (j = 2; j >= i; j--) {
            cr6_mulsmall(&cr6_tmp, &cr6_c[j + 1], a0);
            cr6_add(&cr6_c[j], &cr6_tmp);
        }
    }
    /* x -> 1/y, times y^3: reverse coefficients */
    t = cr6_c[0]; cr6_c[0] = cr6_c[3]; cr6_c[3] = t;
    t = cr6_c[1]; cr6_c[1] = cr6_c[2]; cr6_c[2] = t;
}

seq_t a(idx_t n) {
    seq_t p2, p1;
    idx_t i;
    if (!cr6_init) {
        int k;
        for (k = 0; k < 4; k++) {
            cr6_c[k].d = NULL; cr6_c[k].n = 0; cr6_c[k].cap = 0; cr6_c[k].neg = 0;
        }
        cr6_acc.d = NULL; cr6_acc.n = 0; cr6_acc.cap = 0; cr6_acc.neg = 0;
        cr6_tmp.d = NULL; cr6_tmp.n = 0; cr6_tmp.cap = 0; cr6_tmp.neg = 0;
        cr6_set_small(&cr6_c[0], 6, 1);
        cr6_set_small(&cr6_c[1], 0, 0);
        cr6_set_small(&cr6_c[2], 0, 0);
        cr6_set_small(&cr6_c[3], 1, 0);
        cr6_init = 1;
    }
    while ((idx_t)cr6_len <= n) cr6_step();
    p2 = 1;
    p1 = (seq_t)cr6_q[0];
    for (i = 1; i <= n; i++) {
        seq_t cur = (seq_t)cr6_q[i] * p1 + p2;
        p2 = p1;
        p1 = cur;
    }
    return p1;
}
