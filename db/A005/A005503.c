/*
 * A005503  Number of unrooted triangulations of a disk with one internal node
 *          and n+3 nodes on the boundary.
 * https://oeis.org/A005503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside over the dihedral group of the boundary (m = n+3): a(n)
 *            = ( Sum_{p | m} phi(m/p)*R(p, m/p) + Refl(m) ) / (2m), where
 *            R(p,1) = binomial(2p-1,p) - 3*binomial(2p-3,p-2), R(p,2) =
 *            binomial(2p-1,p) - binomial(2p-2,p-1), R(p,d) = binomial(2p-1,p)
 *            for d >= 3, Refl(m) = m*(2*C(s)-C(s-1)), s=(m-1...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   tot
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/tot.h"

typedef struct { uint32_t *d; size_t cap; } bn_t;

static void bn_init(bn_t *x, size_t cap) {
    x->cap = cap;
    x->d = (uint32_t *)calloc(cap, sizeof(uint32_t));
}
static void bn_free(bn_t *x) { free(x->d); x->d = NULL; }
static void bn_set(bn_t *x, uint32_t v) {
    memset(x->d, 0, x->cap * sizeof(uint32_t));
    x->d[0] = v;
}
static size_t bn_top(const bn_t *x) {
    size_t t = x->cap;
    while (t > 0 && x->d[t - 1] == 0) t--;
    return t;
}
static void bn_mul_small(bn_t *x, uint32_t m) {
    uint64_t carry = 0;
    size_t t = bn_top(x), i;
    for (i = 0; i < t || (carry && i < x->cap); i++) {
        uint64_t v = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)v;
        carry = v >> 32;
    }
}
static void bn_div_small(bn_t *x, uint32_t m) {
    uint64_t rem = 0;
    size_t i = bn_top(x);
    while (i-- > 0) {
        uint64_t v = (rem << 32) | x->d[i];
        x->d[i] = (uint32_t)(v / m);
        rem = v % m;
    }
}
static void bn_add(bn_t *a, const bn_t *b) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < a->cap; i++) {
        uint64_t v = (uint64_t)a->d[i] + b->d[i] + carry;
        a->d[i] = (uint32_t)v;
        carry = v >> 32;
    }
}
static void bn_sub(bn_t *a, const bn_t *b) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < a->cap; i++) {
        int64_t v = (int64_t)a->d[i] - (int64_t)b->d[i] - borrow;
        if (v < 0) { v += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        a->d[i] = (uint32_t)v;
    }
}
/* x = binomial(nn, k) */
static void bn_binom(bn_t *x, uint64_t nn, uint64_t k) {
    uint64_t i;
    bn_set(x, 1);
    if (k > nn) { bn_set(x, 0); return; }
    if (k > nn - k) k = nn - k;
    for (i = 1; i <= k; i++) {
        bn_mul_small(x, (uint32_t)(nn - k + i));
        bn_div_small(x, (uint32_t)i);
    }
}
static void bn_catalan(bn_t *x, uint64_t s) {
    bn_binom(x, 2 * s, s);
    bn_div_small(x, (uint32_t)(s + 1));
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n + 3, p;
    size_t cap = (size_t)(m / 8) + 16, i;
    bn_t sum, t, u;
    seq_t res = 0;
    bn_init(&sum, cap); bn_init(&t, cap); bn_init(&u, cap);
    bn_set(&sum, 0);
    for (p = 1; p <= m; p++) {
        uint64_t d;
        if (m % p) continue;
        d = m / p;
        bn_binom(&t, 2 * p - 1, p);
        if (d == 1) {
            bn_binom(&u, 2 * p - 3, p - 2);
            bn_mul_small(&u, 3);
            bn_sub(&t, &u);
        } else if (d == 2) {
            bn_binom(&u, 2 * p - 2, p - 1);
            bn_sub(&t, &u);
        }
        bn_mul_small(&t, (uint32_t)tot(d));
        bn_add(&sum, &t);
    }
    if (m % 2 == 1) {
        uint64_t s = (m - 1) / 2;
        bn_catalan(&t, s);
        bn_mul_small(&t, 2);
        bn_catalan(&u, s - 1);
        bn_sub(&t, &u);
        bn_mul_small(&t, (uint32_t)m);
    } else {
        uint64_t h = m / 2;
        bn_catalan(&t, h);
        bn_catalan(&u, h - 1);
        bn_sub(&t, &u);
        bn_mul_small(&t, (uint32_t)(2 * m));
    }
    bn_add(&sum, &t);
    bn_div_small(&sum, (uint32_t)(2 * m));
    i = bn_top(&sum);
    while (i-- > 0) {
        int sh;
        for (sh = 24; sh >= 0; sh -= 8) {
            unsigned b = (unsigned)((sum.d[i] >> sh) & 0xFFu);
            res = res * 256 + (seq_t)b;
        }
    }
    bn_free(&sum); bn_free(&t); bn_free(&u);
    return res;
}
