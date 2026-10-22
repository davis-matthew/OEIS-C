/*
 * A007768  From Engel product expansion of 4/7.
 * https://oeis.org/A007768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + floor(1 / ((1-A(n)) * b(n-1))), A(n) = A(n-1) *
 *            b(n-1) / (b(n-1) - 1), b(n) = Product_{k=1..n} a(k), b(0) = 1,
 *            A(1) = 4/7. Implemented via z(n) = (1-A(n))*b(n-1): a(n) = 1 +
 *            floor(1/z(n)), z(1) = 3/7, z(n+1) = b(n)*(a(n)*z(n) - 1) / (b(n)
 *            - 1), using exact internal big integers
 * Range:     u8 n <= 12, u16 n <= 17, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

typedef struct { uint32_t *d; size_t n; } bn;

static bn bn_make(size_t cap) {
    bn r;
    r.d = (uint32_t *)calloc(cap ? cap : 1, sizeof(uint32_t));
    r.n = 0;
    return r;
}

static void bn_norm(bn *x) {
    while (x->n && x->d[x->n - 1] == 0) x->n--;
}

static bn bn_from_u32(uint32_t v) {
    bn r = bn_make(1);
    r.d[0] = v;
    r.n = v ? 1 : 0;
    return r;
}

static bn bn_copy(const bn *x) {
    bn r = bn_make(x->n);
    if (x->n) memcpy(r.d, x->d, x->n * sizeof(uint32_t));
    r.n = x->n;
    return r;
}

static void bn_free(bn *x) {
    free(x->d);
    x->d = NULL;
    x->n = 0;
}

static int bn_cmp(const bn *x, const bn *y) {
    size_t i;
    if (x->n != y->n) return x->n < y->n ? -1 : 1;
    for (i = x->n; i-- > 0;) {
        if (x->d[i] != y->d[i]) return x->d[i] < y->d[i] ? -1 : 1;
    }
    return 0;
}

static bn bn_mul(const bn *x, const bn *y) {
    size_t i, j;
    bn r = bn_make(x->n + y->n);
    for (i = 0; i < x->n; i++) {
        uint64_t carry = 0;
        for (j = 0; j < y->n; j++) {
            uint64_t t = (uint64_t)x->d[i] * y->d[j] + r.d[i + j] + carry;
            r.d[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        r.d[i + y->n] = (uint32_t)carry;
    }
    r.n = x->n + y->n;
    bn_norm(&r);
    return r;
}

/* x -= y, requires x >= y */
static void bn_sub_inplace(bn *x, const bn *y) {
    size_t i;
    int64_t borrow = 0;
    for (i = 0; i < x->n; i++) {
        int64_t yi = (i < y->n) ? (int64_t)y->d[i] : 0;
        int64_t t;
        if (i >= y->n && borrow == 0) break;
        t = (int64_t)x->d[i] - yi - borrow;
        if (t < 0) { t += (int64_t)1 << 32; borrow = 1; } else borrow = 0;
        x->d[i] = (uint32_t)t;
    }
    bn_norm(x);
}

static void bn_inc(bn *x) {
    size_t i = 0;
    x->d = (uint32_t *)realloc(x->d, (x->n + 1) * sizeof(uint32_t));
    x->d[x->n] = 0;
    for (;;) {
        x->d[i]++;
        if (x->d[i] != 0) break;
        i++;
    }
    if (i >= x->n) x->n = i + 1;
}

/* x -= 1, requires x >= 1 */
static void bn_dec(bn *x) {
    size_t i = 0;
    for (;;) {
        if (x->d[i] != 0) { x->d[i]--; break; }
        x->d[i] = 0xFFFFFFFFu;
        i++;
    }
    bn_norm(x);
}

static size_t bn_bitlen(const bn *x) {
    size_t b;
    uint32_t top;
    if (x->n == 0) return 0;
    b = (x->n - 1) * 32;
    top = x->d[x->n - 1];
    while (top) { b++; top >>= 1; }
    return b;
}

static bn bn_shl(const bn *y, size_t k) {
    size_t limbs = k / 32, bits = k % 32, i;
    bn r = bn_make(y->n + limbs + 1);
    for (i = 0; i < y->n; i++) {
        r.d[i + limbs] |= y->d[i] << bits;
        if (bits) r.d[i + limbs + 1] |= y->d[i] >> (32 - bits);
    }
    r.n = y->n + limbs + 1;
    bn_norm(&r);
    return r;
}

static void bn_shr1(bn *x) {
    size_t i;
    for (i = 0; i < x->n; i++) {
        uint32_t hi = (i + 1 < x->n) ? (x->d[i + 1] << 31) : 0u;
        x->d[i] = (x->d[i] >> 1) | hi;
    }
    bn_norm(x);
}

/* floor(x / y), y > 0 */
static bn bn_div(const bn *x, const bn *y) {
    size_t lx = bn_bitlen(x), ly = bn_bitlen(y), k, i;
    bn T, R, Q;
    if (lx < ly) return bn_make(1);
    k = lx - ly;
    T = bn_shl(y, k);
    R = bn_copy(x);
    Q = bn_make(k / 32 + 1);
    Q.n = k / 32 + 1;
    for (i = k + 1; i-- > 0;) {
        if (bn_cmp(&R, &T) >= 0) {
            bn_sub_inplace(&R, &T);
            Q.d[i / 32] |= 1u << (i % 32);
        }
        bn_shr1(&T);
    }
    bn_free(&R);
    bn_free(&T);
    bn_norm(&Q);
    return Q;
}

static bn *g_a = NULL;
static size_t g_cnt = 0, g_cap = 0;
static bn g_p, g_q, g_b;

seq_t a(idx_t n) {
    seq_t r = 0;
    size_t i;
    int s;
    const bn *v;

    if (g_cnt == 0 && g_a == NULL) {
        g_p = bn_from_u32(3);
        g_q = bn_from_u32(7);
        g_b = bn_from_u32(1);
    }
    while ((idx_t)g_cnt < n) {
        bn an, bn_new, t, aq, np, bm1, nq;
        an = bn_div(&g_q, &g_p);
        bn_inc(&an);
        if (g_cnt == g_cap) {
            g_cap = g_cap ? g_cap * 2 : 64;
            g_a = (bn *)realloc(g_a, g_cap * sizeof(bn));
        }
        g_a[g_cnt++] = an;

        bn_new = bn_mul(&g_b, &an);
        t = bn_mul(&an, &g_p);
        bn_sub_inplace(&t, &g_q);
        np = bn_mul(&bn_new, &t);
        bm1 = bn_copy(&bn_new);
        bn_dec(&bm1);
        nq = bn_mul(&g_q, &bm1);
        (void)aq;

        bn_free(&t);
        bn_free(&bm1);
        bn_free(&g_p);
        bn_free(&g_q);
        bn_free(&g_b);
        g_p = np;
        g_q = nq;
        g_b = bn_new;
    }

    v = &g_a[n - 1];
    for (i = v->n; i-- > 0;) {
        uint32_t d = v->d[i];
        for (s = 24; s >= 0; s -= 8) {
            r = r * 256u;
            r += (idx_t)((d >> s) & 255u);
        }
    }
    return r;
}
