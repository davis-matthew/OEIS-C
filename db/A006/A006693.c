/*
 * A006693  Modified Engel expansion of 3/7.
 * https://oeis.org/A006693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(1/A(n)) where A(1) = 3/7, A(n+1) = (1/a(n) - A(n))
 *            * (a(n) + 1). A(n) = P/Q is kept exactly in lowest terms with
 *            private multi-limb integers: a(n) = floor(Q/P), A(n+1) = ((Q mod
 *            P)*(a(n)+1)) / (a(n)*Q), reduced by gcd
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t n, cap; } bn;

static void bn_reserve(bn *x, size_t c) {
    if (c > x->cap) {
        size_t nc = c * 2 + 4;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}

static void bn_trim(bn *x) {
    while (x->n && x->d[x->n - 1] == 0) x->n--;
}

static void bn_set_u32(bn *x, uint32_t v) {
    bn_reserve(x, 1);
    x->d[0] = v;
    x->n = v ? 1 : 0;
}

static void bn_copy(bn *dst, const bn *src) {
    bn_reserve(dst, src->n + 1);
    if (src->n) memcpy(dst->d, src->d, src->n * sizeof(uint32_t));
    dst->n = src->n;
}

static int bn_cmp(const bn *a, const bn *b) {
    size_t i;
    if (a->n != b->n) return a->n < b->n ? -1 : 1;
    for (i = a->n; i-- > 0;)
        if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    return 0;
}

/* a -= b, requires a >= b */
static void bn_sub(bn *a, const bn *b) {
    uint64_t borrow = 0;
    size_t i;
    for (i = 0; i < a->n; i++) {
        uint64_t bi = i < b->n ? b->d[i] : 0;
        uint64_t t = (uint64_t)a->d[i] - bi - borrow;
        a->d[i] = (uint32_t)t;
        borrow = (t >> 63) & 1;
    }
    bn_trim(a);
}

static void bn_add1(bn *x) {
    size_t i;
    for (i = 0; i < x->n; i++) {
        x->d[i]++;
        if (x->d[i] != 0) return;
    }
    bn_reserve(x, x->n + 1);
    x->d[x->n++] = 1;
}

/* r = a * b, r distinct from a and b */
static void bn_mul(bn *r, const bn *a, const bn *b) {
    size_t i, j;
    if (a->n == 0 || b->n == 0) { r->n = 0; return; }
    bn_reserve(r, a->n + b->n);
    memset(r->d, 0, (a->n + b->n) * sizeof(uint32_t));
    for (i = 0; i < a->n; i++) {
        uint64_t carry = 0;
        for (j = 0; j < b->n; j++) {
            uint64_t t = (uint64_t)a->d[i] * b->d[j] + r->d[i + j] + carry;
            r->d[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        r->d[i + b->n] = (uint32_t)carry;
    }
    r->n = a->n + b->n;
    bn_trim(r);
}

static void bn_shl1_or(bn *x, uint32_t bit) {
    uint32_t carry = bit;
    size_t i;
    for (i = 0; i < x->n; i++) {
        uint32_t v = x->d[i];
        x->d[i] = (v << 1) | carry;
        carry = v >> 31;
    }
    if (carry) {
        bn_reserve(x, x->n + 1);
        x->d[x->n++] = carry;
    }
}

static void bn_shr_bits(bn *x, size_t k) {
    size_t L = k / 32, i;
    unsigned b = (unsigned)(k % 32);
    if (k == 0) return;
    if (L >= x->n) { x->n = 0; return; }
    if (L) {
        for (i = 0; i < x->n - L; i++) x->d[i] = x->d[i + L];
        x->n -= L;
    }
    if (b) {
        for (i = 0; i < x->n; i++)
            x->d[i] = (x->d[i] >> b) | (i + 1 < x->n ? x->d[i + 1] << (32 - b) : 0);
    }
    bn_trim(x);
}

static size_t bn_ctz(const bn *x) {
    size_t i = 0, c = 0;
    uint32_t v;
    while (x->d[i] == 0) i++;
    v = x->d[i];
    while (!(v & 1)) { v >>= 1; c++; }
    return i * 32 + c;
}

static size_t bn_bits(const bn *x) {
    uint32_t top;
    size_t c = 0;
    if (x->n == 0) return 0;
    top = x->d[x->n - 1];
    while (top) { top >>= 1; c++; }
    return 32 * (x->n - 1) + c;
}

/* q = floor(a / b), r = a mod b; q, r distinct from a, b */
static void bn_divmod(bn *q, bn *r, const bn *a, const bn *b) {
    size_t i = bn_bits(a);
    bn_reserve(q, a->n + 1);
    if (a->n) memset(q->d, 0, a->n * sizeof(uint32_t));
    q->n = a->n;
    r->n = 0;
    while (i-- > 0) {
        bn_shl1_or(r, (a->d[i / 32] >> (i % 32)) & 1u);
        if (bn_cmp(r, b) >= 0) {
            bn_sub(r, b);
            q->d[i / 32] |= 1u << (i % 32);
        }
    }
    bn_trim(q);
}

static bn bP = {0, 0, 0}, bQ = {0, 0, 0};
static bn tA = {0, 0, 0}, tR = {0, 0, 0}, tA1 = {0, 0, 0};
static bn tN = {0, 0, 0}, tD = {0, 0, 0}, tU = {0, 0, 0}, tV = {0, 0, 0};
static bn *terms = 0;
static size_t nterms = 0, termcap = 0;
static int inited = 0;
static int finished = 0;

static void step(void) {
    size_t zu, zv, s;
    bn_divmod(&tA, &tR, &bQ, &bP);
    if (nterms == termcap) {
        size_t nc = termcap * 2 + 16, i;
        terms = (bn *)realloc(terms, nc * sizeof(bn));
        for (i = termcap; i < nc; i++) { terms[i].d = 0; terms[i].n = 0; terms[i].cap = 0; }
        termcap = nc;
    }
    bn_copy(&terms[nterms], &tA);
    nterms++;
    if (tR.n == 0) { finished = 1; return; }

    bn_copy(&tA1, &tA);
    bn_add1(&tA1);
    bn_mul(&tN, &tR, &tA1);
    bn_mul(&tD, &tA, &bQ);

    /* binary gcd of tN and tD */
    bn_copy(&tU, &tN);
    bn_copy(&tV, &tD);
    zu = bn_ctz(&tU);
    zv = bn_ctz(&tV);
    s = zu < zv ? zu : zv;
    bn_shr_bits(&tU, zu);
    while (tV.n) {
        bn_shr_bits(&tV, bn_ctz(&tV));
        if (bn_cmp(&tU, &tV) > 0) { bn t = tU; tU = tV; tV = t; }
        bn_sub(&tV, &tU);
    }
    bn_shr_bits(&tN, s);
    bn_shr_bits(&tD, s);
    bn_divmod(&bP, &tR, &tN, &tU);
    bn_divmod(&bQ, &tR, &tD, &tU);
}

static seq_t bn_to_seq(const bn *x) {
    seq_t r = 0;
    size_t i;
    int k;
    for (i = x->n; i-- > 0;) {
        for (k = 3; k >= 0; k--) {
            unsigned byte = (unsigned)((x->d[i] >> (8 * k)) & 0xFFu);
            r = (seq_t)(r * 256);
            r = (seq_t)(r + byte);
        }
    }
    return r;
}

seq_t a(idx_t n) {
    if (!inited) {
        bn_set_u32(&bP, 3);
        bn_set_u32(&bQ, 7);
        inited = 1;
    }
    if (n < 1) return 0;
    while (nterms < n && !finished) step();
    if (n > nterms) return 0;
    return bn_to_seq(&terms[n - 1]);
}
