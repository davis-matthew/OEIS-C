/*
 * A005481  Decimal expansion of cube root of 5.
 * https://oeis.org/A005481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(5^(1/3) * 10^(n-1)) mod 10, i.e. the n-th digit of
 *            floor(cbrt(5*10^(3(n-1)))), computed by the digit-by-digit
 *            cube-root method on native base-10^9 bignums (each step: largest
 *            d with 300*r^2*d + 30*r*d^2 + d^3 <= remainder)
 * Range:     u8 n <= 9389, u16 n <= 9389, u32 n <= 9389, u64 n <= 9389,
 *            u128 n <= 9389, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BN_BASE 1000000000ULL

typedef struct { uint64_t *v; size_t len, cap; } bn;

static void bn_fit(bn *x, size_t n) {
    if (n > x->cap) {
        size_t c = x->cap ? x->cap * 2 : 16;
        while (c < n) c *= 2;
        x->v = (uint64_t *)realloc(x->v, c * sizeof(uint64_t));
        x->cap = c;
    }
}

static void bn_trim(bn *x) {
    while (x->len && x->v[x->len - 1] == 0) x->len--;
}

/* x = x*m + a  (m, a small) */
static void bn_muladd(bn *x, uint64_t m, uint64_t a) {
    uint64_t c = a;
    size_t i;
    for (i = 0; i < x->len; i++) {
        uint64_t t = x->v[i] * m + c;
        x->v[i] = t % BN_BASE;
        c = t / BN_BASE;
    }
    while (c) {
        bn_fit(x, x->len + 1);
        x->v[x->len++] = c % BN_BASE;
        c /= BN_BASE;
    }
    bn_trim(x);
}

static void bn_set(bn *d, const bn *s) {
    bn_fit(d, s->len + 1);
    if (s->len) memcpy(d->v, s->v, s->len * sizeof(uint64_t));
    d->len = s->len;
}

static void bn_add(bn *x, const bn *y) {
    size_t n = x->len > y->len ? x->len : y->len;
    size_t i;
    uint64_t c = 0;
    bn_fit(x, n + 1);
    for (i = x->len; i < n; i++) x->v[i] = 0;
    x->len = n;
    for (i = 0; i < n; i++) {
        uint64_t t = x->v[i] + (i < y->len ? y->v[i] : 0) + c;
        if (t >= BN_BASE) { x->v[i] = t - BN_BASE; c = 1; }
        else { x->v[i] = t; c = 0; }
    }
    if (c) x->v[x->len++] = c;
}

/* x -= y, requires x >= y */
static void bn_sub(bn *x, const bn *y) {
    size_t i;
    uint64_t b = 0;
    for (i = 0; i < x->len; i++) {
        uint64_t s = (i < y->len ? y->v[i] : 0) + b;
        if (x->v[i] >= s) { x->v[i] -= s; b = 0; }
        else { x->v[i] = x->v[i] + BN_BASE - s; b = 1; }
    }
    bn_trim(x);
}

static int bn_cmp(const bn *x, const bn *y) {
    size_t i;
    if (x->len != y->len) return x->len < y->len ? -1 : 1;
    for (i = x->len; i-- > 0;) {
        if (x->v[i] != y->v[i]) return x->v[i] < y->v[i] ? -1 : 1;
    }
    return 0;
}

static bn st_r, st_s, st_rem, st_A, st_B, st_T, st_tmp;
static unsigned char *st_dig = NULL;
static size_t st_ndig = 0, st_dcap = 0;

static void eval_T(uint64_t d) {
    bn_set(&st_T, &st_A);
    bn_muladd(&st_T, d, d * d * d);
    bn_set(&st_tmp, &st_B);
    bn_muladd(&st_tmp, d * d, 0);
    bn_add(&st_T, &st_tmp);
}

static void next_digit(void) {
    uint64_t group = (st_ndig == 0) ? 5 : 0;
    uint64_t lo = 0, hi = 9, d;

    bn_muladd(&st_rem, 1000, group);
    bn_set(&st_A, &st_s);
    bn_muladd(&st_A, 300, 0);
    bn_set(&st_B, &st_r);
    bn_muladd(&st_B, 30, 0);

    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) / 2;
        eval_T(mid);
        if (bn_cmp(&st_T, &st_rem) <= 0) lo = mid;
        else hi = mid - 1;
    }
    d = lo;
    eval_T(d);
    bn_sub(&st_rem, &st_T);

    /* s = 100*s + 20*r*d + d^2 ; r = 10*r + d */
    bn_set(&st_tmp, &st_r);
    bn_muladd(&st_tmp, 20 * d, 0);
    bn_muladd(&st_s, 100, d * d);
    bn_add(&st_s, &st_tmp);
    bn_muladd(&st_r, 10, d);

    if (st_ndig + 1 > st_dcap) {
        size_t c = st_dcap ? st_dcap * 2 : 1024;
        st_dig = (unsigned char *)realloc(st_dig, c);
        st_dcap = c;
    }
    st_dig[st_ndig++] = (unsigned char)d;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    while (st_ndig < (size_t)n) next_digit();
    return (seq_t)st_dig[n - 1];
}
