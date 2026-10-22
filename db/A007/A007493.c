/*
 * A007493  Decimal expansion of Wallis's number, the real root of x^3 - 2*x -
 *          5.
 * https://oeis.org/A007493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   digit-by-digit search for the real root r of x^3 - 2*x - 5 = 0.
 *            With D = 10^k and X = floor(r*D), keep G = -(X^3 - 2*X*D^2 -
 *            5*D^3) >= 0. The next digit d is the largest one with d*(300*X^2
 *            - 200*D^2) + 30*d^2*X + d^3 <= 1000*G. Then a(k+2) = d, and a(1)
 *            = 2
 * Range:     u8 n <= 9751, u16 n <= 9751, u32 n <= 9751, u64 n <= 9751,
 *            u128 n <= 9751, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BN_BASE 1000000000u

typedef struct { uint32_t *v; size_t len; size_t cap; } bn;

static void bn_reserve(bn *r, size_t cap) {
    if (r->cap < cap) {
        r->v = (uint32_t *)realloc(r->v, cap * sizeof(uint32_t));
        memset(r->v + r->cap, 0, (cap - r->cap) * sizeof(uint32_t));
        r->cap = cap;
    }
}

static void bn_trim(bn *r) {
    while (r->len > 1 && r->v[r->len - 1] == 0) r->len--;
}

static void bn_set(bn *r, uint32_t s) {
    r->v[0] = s;
    r->len = 1;
}

static void bn_copy(bn *r, const bn *a) {
    memcpy(r->v, a->v, a->len * sizeof(uint32_t));
    r->len = a->len;
}

/* r = a * m (r may alias a) */
static void bn_muls(bn *r, const bn *a, uint32_t m) {
    uint64_t carry = 0;
    size_t i, n = a->len;
    for (i = 0; i < n; i++) {
        uint64_t t = (uint64_t)a->v[i] * m + carry;
        r->v[i] = (uint32_t)(t % BN_BASE);
        carry = t / BN_BASE;
    }
    while (carry) {
        r->v[n++] = (uint32_t)(carry % BN_BASE);
        carry /= BN_BASE;
    }
    r->len = n;
    bn_trim(r);
}

/* r += a * m */
static void bn_addmul(bn *r, const bn *a, uint32_t m) {
    uint64_t carry = 0;
    size_t i;
    while (r->len < a->len) r->v[r->len++] = 0;
    for (i = 0; i < a->len; i++) {
        uint64_t t = (uint64_t)r->v[i] + (uint64_t)a->v[i] * m + carry;
        r->v[i] = (uint32_t)(t % BN_BASE);
        carry = t / BN_BASE;
    }
    while (carry) {
        if (i >= r->len) r->v[r->len++] = 0;
        {
            uint64_t t = (uint64_t)r->v[i] + carry;
            r->v[i] = (uint32_t)(t % BN_BASE);
            carry = t / BN_BASE;
        }
        i++;
    }
    bn_trim(r);
}

static void bn_addsmall(bn *r, uint32_t s) {
    uint64_t carry = s;
    size_t i = 0;
    while (carry) {
        if (i >= r->len) r->v[r->len++] = 0;
        {
            uint64_t t = (uint64_t)r->v[i] + carry;
            r->v[i] = (uint32_t)(t % BN_BASE);
            carry = t / BN_BASE;
        }
        i++;
    }
}

/* r -= a, requires r >= a */
static void bn_sub(bn *r, const bn *a) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < r->len; i++) {
        int64_t t = (int64_t)r->v[i] - borrow - (i < a->len ? (int64_t)a->v[i] : 0);
        if (t < 0) { t += BN_BASE; borrow = 1; } else borrow = 0;
        r->v[i] = (uint32_t)t;
    }
    bn_trim(r);
}

static int bn_cmp(const bn *a, const bn *b) {
    size_t i;
    if (a->len != b->len) return a->len < b->len ? -1 : 1;
    for (i = a->len; i-- > 0;) {
        if (a->v[i] != b->v[i]) return a->v[i] < b->v[i] ? -1 : 1;
    }
    return 0;
}

static bn sX, sG, sP, sQ, sT, sH, sS, sTmp;
static unsigned char *digs = NULL;
static size_t ndig = 0, digcap = 0;

/* S = d*T + 30*d^2*X + d^3 */
static void eval_S(uint32_t d) {
    bn_muls(&sS, &sT, d);
    bn_addmul(&sS, &sX, 30u * d * d);
    bn_addsmall(&sS, d * d * d);
}

static void extend_to(size_t target) {
    size_t cap = (3 * target) / 9 + 16;
    bn_reserve(&sX, cap); bn_reserve(&sG, cap); bn_reserve(&sP, cap);
    bn_reserve(&sQ, cap); bn_reserve(&sT, cap); bn_reserve(&sH, cap);
    bn_reserve(&sS, cap); bn_reserve(&sTmp, cap);
    if (digcap < target) {
        digs = (unsigned char *)realloc(digs, target);
        digcap = target;
    }
    if (ndig == 0) {
        bn_set(&sX, 2); bn_set(&sG, 1); bn_set(&sP, 4); bn_set(&sQ, 1);
        digs[0] = 2;
        ndig = 1;
    }
    while (ndig < target) {
        uint32_t lo = 0, hi = 9, d;
        bn_muls(&sT, &sP, 300);
        bn_muls(&sTmp, &sQ, 200);
        bn_sub(&sT, &sTmp);
        bn_muls(&sH, &sG, 1000);
        while (lo < hi) {
            uint32_t mid = (lo + hi + 1) / 2;
            eval_S(mid);
            if (bn_cmp(&sS, &sH) <= 0) lo = mid; else hi = mid - 1;
        }
        d = lo;
        eval_S(d);
        bn_copy(&sG, &sH);
        bn_sub(&sG, &sS);
        bn_muls(&sP, &sP, 100);
        bn_addmul(&sP, &sX, 20u * d);
        bn_addsmall(&sP, d * d);
        bn_muls(&sX, &sX, 10);
        bn_addsmall(&sX, d);
        bn_muls(&sQ, &sQ, 100);
        digs[ndig++] = (unsigned char)d;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if ((size_t)n > ndig) {
        size_t target = (size_t)n;
        if (target < 2 * ndig) target = 2 * ndig;
        if (target < 64) target = 64;
        extend_to(target);
    }
    return (seq_t)digs[n - 1];
}
