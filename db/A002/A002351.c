/*
 * A002351  Denominators of convergents to cube root of 2.
 * https://oeis.org/A002351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), a(-1) = 0, a(0) = 1, where c(0),
 *            c(1), ... = 1, 3, 1, 5, 1, 1, 4, ... are the partial quotients
 *            of 2^(1/3). They are found exactly with Lagrange's method: keep
 *            a cubic P whose only real root is the current complete quotient
 *            x > 1, take c = floor(x) by checking the sig...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 34, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t n, cap; int neg; } a002351_bz;

static void a002351_reserve(a002351_bz *x, size_t c) {
    if (c > x->cap) {
        size_t nc = c * 2 + 4;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}

static void a002351_trim(a002351_bz *x) {
    while (x->n && x->d[x->n - 1] == 0) x->n--;
    if (!x->n) x->neg = 0;
}

static void a002351_copy(a002351_bz *r, const a002351_bz *x) {
    a002351_reserve(r, x->n + 1);
    if (x->n) memcpy(r->d, x->d, x->n * sizeof(uint32_t));
    r->n = x->n;
    r->neg = x->neg;
}

static void a002351_set(a002351_bz *r, int v) {
    a002351_reserve(r, 1);
    r->neg = v < 0;
    r->d[0] = (uint32_t)(v < 0 ? -v : v);
    r->n = v ? 1 : 0;
}

static int a002351_magcmp(const a002351_bz *x, const a002351_bz *y) {
    size_t i;
    if (x->n != y->n) return x->n < y->n ? -1 : 1;
    for (i = x->n; i-- > 0;)
        if (x->d[i] != y->d[i]) return x->d[i] < y->d[i] ? -1 : 1;
    return 0;
}

static void a002351_magadd(a002351_bz *r, const a002351_bz *x) {
    size_t n = r->n > x->n ? r->n : x->n, i;
    uint64_t carry = 0;
    a002351_reserve(r, n + 1);
    for (i = r->n; i < n; i++) r->d[i] = 0;
    for (i = 0; i < n; i++) {
        uint64_t t = (uint64_t)r->d[i] + (i < x->n ? x->d[i] : 0) + carry;
        r->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    r->n = n;
    if (carry) r->d[r->n++] = (uint32_t)carry;
}

/* |r| = |r| - |x|, requires |r| >= |x| */
static void a002351_magsub(a002351_bz *r, const a002351_bz *x) {
    size_t i;
    uint64_t borrow = 0;
    for (i = 0; i < r->n; i++) {
        uint64_t t = (uint64_t)r->d[i] - (i < x->n ? x->d[i] : 0) - borrow;
        r->d[i] = (uint32_t)t;
        borrow = t >> 63;
    }
}

/* |r| = |x| - |r|, requires |x| >= |r| */
static void a002351_magrsub(a002351_bz *r, const a002351_bz *x) {
    size_t i;
    uint64_t borrow = 0;
    a002351_reserve(r, x->n + 1);
    for (i = 0; i < x->n; i++) {
        uint64_t ri = i < r->n ? r->d[i] : 0;
        uint64_t t = (uint64_t)x->d[i] - ri - borrow;
        r->d[i] = (uint32_t)t;
        borrow = t >> 63;
    }
    r->n = x->n;
}

static void a002351_add(a002351_bz *r, const a002351_bz *x) {
    if (x->n == 0) return;
    if (r->n == 0) { a002351_copy(r, x); return; }
    if (r->neg == x->neg) {
        a002351_magadd(r, x);
    } else if (a002351_magcmp(r, x) >= 0) {
        a002351_magsub(r, x);
    } else {
        a002351_magrsub(r, x);
        r->neg = x->neg;
    }
    a002351_trim(r);
}

static void a002351_mulsmall(a002351_bz *r, uint32_t m) {
    size_t i;
    uint64_t carry = 0;
    if (m == 0) { r->n = 0; r->neg = 0; return; }
    a002351_reserve(r, r->n + 1);
    for (i = 0; i < r->n; i++) {
        uint64_t t = (uint64_t)r->d[i] * m + carry;
        r->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) r->d[r->n++] = (uint32_t)carry;
}

static a002351_bz a002351_P[4];   /* P[0]*x^3 + P[1]*x^2 + P[2]*x + P[3] */
static a002351_bz a002351_T, a002351_U;
static uint64_t *a002351_cf = NULL;
static size_t a002351_cnt = 0, a002351_cap = 0;
static int a002351_init = 0;

static int a002351_sign_at(uint32_t c) {
    int j;
    a002351_copy(&a002351_T, &a002351_P[0]);
    for (j = 1; j < 4; j++) {
        a002351_mulsmall(&a002351_T, c);
        a002351_add(&a002351_T, &a002351_P[j]);
    }
    if (a002351_T.n == 0) return 0;
    return a002351_T.neg ? -1 : 1;
}

static void a002351_step(void) {
    int s1 = a002351_sign_at(1), i, j;
    uint64_t lo = 1, hi;
    uint32_t c;
    a002351_bz tmp;
    while (lo < ((uint64_t)1 << 31) && a002351_sign_at((uint32_t)(2 * lo)) == s1) lo *= 2;
    hi = 2 * lo;
    while (hi - lo > 1) {
        uint64_t mid = (lo + hi) / 2;
        if (a002351_sign_at((uint32_t)mid) == s1) lo = mid; else hi = mid;
    }
    c = (uint32_t)lo;

    if (a002351_cnt == a002351_cap) {
        a002351_cap = a002351_cap * 2 + 64;
        a002351_cf = (uint64_t *)realloc(a002351_cf, a002351_cap * sizeof(uint64_t));
    }
    a002351_cf[a002351_cnt++] = c;

    /* Taylor shift x -> x + c */
    for (i = 0; i < 3; i++)
        for (j = 1; j <= 3 - i; j++) {
            a002351_copy(&a002351_U, &a002351_P[j - 1]);
            a002351_mulsmall(&a002351_U, c);
            a002351_add(&a002351_P[j], &a002351_U);
        }
    /* x -> 1/y : reverse coefficients */
    tmp = a002351_P[0]; a002351_P[0] = a002351_P[3]; a002351_P[3] = tmp;
    tmp = a002351_P[1]; a002351_P[1] = a002351_P[2]; a002351_P[2] = tmp;
}

seq_t a(idx_t n) {
    seq_t q2, q1, q;
    idx_t i;
    if (!a002351_init) {
        a002351_set(&a002351_P[0], 1);
        a002351_set(&a002351_P[1], 0);
        a002351_set(&a002351_P[2], 0);
        a002351_set(&a002351_P[3], -2);
        a002351_init = 1;
    }
    while ((idx_t)a002351_cnt <= n) a002351_step();

    q2 = 0;
    q1 = 1;
    for (i = 1; i <= n; i++) {
        q = (seq_t)a002351_cf[i] * q1 + q2;
        q2 = q1;
        q1 = q;
    }
    return q1;
}
