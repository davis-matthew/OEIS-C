/*
 * A007140  Number of unlabeled bicolored graphs, with no isolated nodes, on
 *          2n nodes having n nodes of each color and allowing the color
 *          classes to be interchanged.
 * https://oeis.org/A007140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A054976(n) + A319155(n)) / 2, computed by Burnside/Polya
 *            counting. B(p,q) = Sum_{lambda|-p, mu|-q} 2^(Sum_{i,j}
 *            gcd(lambda_i,mu_j)) / (z_lambda z_mu) counts bicolored graphs
 *            (isolated nodes allowed), so A054976(n) = B(n,n) - 2 B(n,n-1) +
 *            B(n-1,n-1). S(n) = Sum_{lambda|-n} 2^(Sum_k ceil(la...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   a7140_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a7140_gcd.h"

typedef struct { uint32_t *d; size_t len, cap; } bn_t;

static void bn_init(bn_t *a) { a->d = NULL; a->len = 0; a->cap = 0; }
static void bn_free(bn_t *a) { free(a->d); bn_init(a); }
static void bn_reserve(bn_t *a, size_t c)
{
    if (a->cap < c) {
        size_t nc = c * 2;
        a->d = (uint32_t *)realloc(a->d, nc * sizeof(uint32_t));
        a->cap = nc;
    }
}
static void bn_trim(bn_t *a) { while (a->len && a->d[a->len - 1] == 0) a->len--; }
static void bn_set_u(bn_t *a, uint32_t v)
{
    bn_reserve(a, 1);
    a->len = 0;
    if (v) { a->d[0] = v; a->len = 1; }
}
static void bn_copy(bn_t *a, const bn_t *b)
{
    bn_reserve(a, b->len + 1);
    if (b->len) memcpy(a->d, b->d, b->len * sizeof(uint32_t));
    a->len = b->len;
}
static void bn_mul_small(bn_t *a, uint32_t m)
{
    uint64_t carry = 0;
    size_t i;
    if (m == 0) { a->len = 0; return; }
    for (i = 0; i < a->len; i++) {
        uint64_t t = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) {
        bn_reserve(a, a->len + 1);
        a->d[a->len++] = (uint32_t)carry;
    }
}
/* a += x << sh */
static void bn_add_shl(bn_t *a, const bn_t *x, unsigned sh)
{
    size_t q, n, m, i;
    unsigned r;
    uint64_t carry = 0;
    if (!x->len) return;
    q = sh / 32; r = sh % 32;
    n = x->len + q + 1;
    m = (a->len > n ? a->len : n) + 1;
    bn_reserve(a, m);
    for (i = a->len; i < m; i++) a->d[i] = 0;
    for (i = 0; i < x->len + 1; i++) {
        uint32_t lo = i < x->len ? x->d[i] : 0;
        uint32_t prev = i ? x->d[i - 1] : 0;
        uint32_t v = r ? ((lo << r) | (prev >> (32 - r))) : lo;
        uint64_t t = (uint64_t)a->d[q + i] + v + carry;
        a->d[q + i] = (uint32_t)t;
        carry = t >> 32;
    }
    for (i = q + x->len + 1; carry && i < m; i++) {
        uint64_t t = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    a->len = m;
    bn_trim(a);
}
/* a -= x, requires a >= x */
static void bn_sub(bn_t *a, const bn_t *x)
{
    size_t i;
    int64_t borrow = 0;
    for (i = 0; i < a->len; i++) {
        int64_t t = (int64_t)a->d[i] - (i < x->len ? (int64_t)x->d[i] : 0) - borrow;
        if (t < 0) { t += (int64_t)1 << 32; borrow = 1; } else borrow = 0;
        a->d[i] = (uint32_t)t;
    }
    bn_trim(a);
}
static void bn_mul(bn_t *res, const bn_t *a, const bn_t *b)
{
    size_t i, j, n;
    if (!a->len || !b->len) { res->len = 0; return; }
    n = a->len + b->len;
    bn_reserve(res, n);
    for (i = 0; i < n; i++) res->d[i] = 0;
    for (i = 0; i < a->len; i++) {
        uint64_t carry = 0;
        for (j = 0; j < b->len; j++) {
            uint64_t t = (uint64_t)a->d[i] * b->d[j] + res->d[i + j] + carry;
            res->d[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        res->d[i + b->len] = (uint32_t)carry;
    }
    res->len = n;
    bn_trim(res);
}
static void bn_div_small(bn_t *a, uint32_t m)
{
    uint64_t rem = 0;
    size_t i = a->len;
    while (i-- > 0) {
        uint64_t cur = (rem << 32) | a->d[i];
        a->d[i] = (uint32_t)(cur / m);
        rem = cur % m;
    }
    bn_trim(a);
}
static seq_t bn_to_seq(const bn_t *a)
{
    seq_t r = 0;
    size_t i = a->len;
    while (i-- > 0) {
        r = r * 65536;
        r = r * 65536;
        r += (idx_t)a->d[i];
    }
    return r;
}

typedef struct {
    unsigned p;
    unsigned *vals, *mults;
    unsigned cnt;
    unsigned *e;
    bn_t *G;
    bn_t fact, w, tmp;
    bn_t accP, accP1, accS;
} a7140_ctx;

static void a7140_process(a7140_ctx *c)
{
    unsigned i, j, k, m, t, s = 0;
    unsigned p = c->p;
    bn_copy(&c->w, &c->fact);
    for (i = 0; i < c->cnt; i++) {
        for (t = 0; t < c->mults[i]; t++) bn_div_small(&c->w, c->vals[i]);
        for (t = 2; t <= c->mults[i]; t++) bn_div_small(&c->w, t);
    }
    /* swap-invariant exponent */
    for (i = 0; i < c->cnt; i++) {
        unsigned v = c->vals[i], cm = c->mults[i];
        s += cm * ((v + 1) / 2) + cm * (cm - 1) / 2 * v;
        for (j = i + 1; j < c->cnt; j++)
            s += cm * c->mults[j] * a7140_gcd(v, c->vals[j]);
    }
    bn_add_shl(&c->accS, &c->w, s);
    /* exponents e[k] = Sum_i gcd(lambda_i, k) */
    for (k = 1; k <= p; k++) {
        unsigned ek = 0;
        for (i = 0; i < c->cnt; i++) ek += c->mults[i] * a7140_gcd(c->vals[i], k);
        c->e[k] = ek;
    }
    bn_set_u(&c->G[0], 1);
    for (m = 1; m <= p; m++) {
        bn_t *g = &c->G[m];
        g->len = 0;
        bn_add_shl(g, &c->G[0], c->e[m]);
        for (j = 1; j < m; j++) {
            bn_mul_small(g, j);
            bn_add_shl(g, &c->G[j], c->e[m - j]);
        }
    }
    bn_mul(&c->tmp, &c->w, &c->G[p]);
    bn_add_shl(&c->accP, &c->tmp, 0);
    if (p >= 1) {
        bn_mul(&c->tmp, &c->w, &c->G[p - 1]);
        bn_add_shl(&c->accP1, &c->tmp, 0);
    }
}

static void a7140_gen(a7140_ctx *c, unsigned rem, unsigned maxpart)
{
    unsigned v, cm;
    if (rem == 0) { a7140_process(c); return; }
    for (v = (rem < maxpart ? rem : maxpart); v >= 1; v--) {
        for (cm = 1; cm * v <= rem; cm++) {
            c->vals[c->cnt] = v;
            c->mults[c->cnt] = cm;
            c->cnt++;
            a7140_gen(c, rem - cm * v, v - 1);
            c->cnt--;
        }
    }
}

/* accP = Sum w*G_p, accP1 = Sum w*G_{p-1}, accS = Sum w*2^s over partitions of p */
static void a7140_pass(unsigned p, bn_t *outP, bn_t *outP1, bn_t *outS)
{
    a7140_ctx c;
    unsigned i;
    c.p = p;
    c.vals = (unsigned *)malloc((p + 1) * sizeof(unsigned));
    c.mults = (unsigned *)malloc((p + 1) * sizeof(unsigned));
    c.e = (unsigned *)malloc((p + 1) * sizeof(unsigned));
    c.G = (bn_t *)malloc((p + 1) * sizeof(bn_t));
    for (i = 0; i <= p; i++) bn_init(&c.G[i]);
    c.cnt = 0;
    bn_init(&c.fact); bn_init(&c.w); bn_init(&c.tmp);
    bn_init(&c.accP); bn_init(&c.accP1); bn_init(&c.accS);
    bn_set_u(&c.fact, 1);
    for (i = 2; i <= p; i++) bn_mul_small(&c.fact, i);
    a7140_gen(&c, p, p);
    bn_copy(outP, &c.accP);
    bn_copy(outP1, &c.accP1);
    bn_copy(outS, &c.accS);
    for (i = 0; i <= p; i++) bn_free(&c.G[i]);
    free(c.G); free(c.vals); free(c.mults); free(c.e);
    bn_free(&c.fact); bn_free(&c.w); bn_free(&c.tmp);
    bn_free(&c.accP); bn_free(&c.accP1); bn_free(&c.accS);
}

seq_t a(idx_t n)
{
    bn_t X1, X2, X3, Y1, Y2, Y3, R;
    unsigned nn, k;
    seq_t res;
    if (n == 0) return 1;
    nn = (unsigned)n;
    bn_init(&X1); bn_init(&X2); bn_init(&X3);
    bn_init(&Y1); bn_init(&Y2); bn_init(&Y3); bn_init(&R);
    a7140_pass(nn, &X1, &X2, &X3);
    a7140_pass(nn - 1, &Y1, &Y2, &Y3);
    /* B(n,n) = X1/(n!)^2, B(n,n-1) = X2/(n!(n-1)!), S(n) = X3/n! */
    for (k = 2; k <= nn; k++) {
        bn_div_small(&X1, k);
        bn_div_small(&X1, k);
        bn_div_small(&X2, k);
        bn_div_small(&X3, k);
    }
    /* B(n-1,n-1) = Y1/((n-1)!)^2, S(n-1) = Y3/(n-1)! */
    for (k = 2; k + 1 <= nn; k++) {
        bn_div_small(&X2, k);
        bn_div_small(&Y1, k);
        bn_div_small(&Y1, k);
        bn_div_small(&Y3, k);
    }
    bn_set_u(&R, 0);
    bn_add_shl(&R, &X1, 0);
    bn_add_shl(&R, &Y1, 0);
    bn_add_shl(&R, &X3, 0);
    bn_sub(&R, &X2);
    bn_sub(&R, &X2);
    bn_sub(&R, &Y3);
    bn_div_small(&R, 2);
    res = bn_to_seq(&R);
    bn_free(&X1); bn_free(&X2); bn_free(&X3);
    bn_free(&Y1); bn_free(&Y2); bn_free(&Y3); bn_free(&R);
    return res;
}
