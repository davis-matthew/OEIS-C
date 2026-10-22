/*
 * A005394  Leading term of Stirling's approximation to n!,
 *          sqrt(2*Pi)*n^(n+(1/2))/e^n, rounded to nearest integer.
 * https://oeis.org/A005394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(sqrt(2*Pi*n)*n^n/e^n), a(0) = 0; computed exactly
 *            with fixed-point bignums (e^-1 by series, Pi by Machin)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t len; } bn;

static bn bn_make(size_t cap) {
    bn r;
    r.d = (uint32_t *)calloc(cap ? cap : 1, sizeof(uint32_t));
    r.len = 0;
    return r;
}
static void bn_free(bn *x) { free(x->d); x->d = NULL; x->len = 0; }
static void bn_trim(bn *x) { while (x->len && x->d[x->len - 1] == 0) x->len--; }
static bn bn_u64(uint64_t v) {
    bn r = bn_make(2);
    r.d[0] = (uint32_t)v; r.d[1] = (uint32_t)(v >> 32); r.len = 2;
    bn_trim(&r);
    return r;
}
static bn bn_copy(const bn *x) {
    bn r = bn_make(x->len);
    if (x->len) memcpy(r.d, x->d, x->len * sizeof(uint32_t));
    r.len = x->len;
    return r;
}
static size_t bn_bits(const bn *x) {
    if (!x->len) return 0;
    uint32_t t = x->d[x->len - 1];
    size_t b = (x->len - 1) * 32;
    while (t) { b++; t >>= 1; }
    return b;
}
static int bn_cmp(const bn *x, const bn *y) {
    if (x->len != y->len) return x->len < y->len ? -1 : 1;
    for (size_t i = x->len; i-- > 0;)
        if (x->d[i] != y->d[i]) return x->d[i] < y->d[i] ? -1 : 1;
    return 0;
}
static bn bn_shl(const bn *x, size_t s) {
    if (!x->len) return bn_make(1);
    size_t w = s / 32, b = s % 32;
    bn r = bn_make(x->len + w + 1);
    for (size_t i = 0; i < x->len; i++) {
        uint64_t v = (uint64_t)x->d[i] << b;
        r.d[i + w] |= (uint32_t)v;
        r.d[i + w + 1] |= (uint32_t)(v >> 32);
    }
    r.len = x->len + w + 1;
    bn_trim(&r);
    return r;
}
static bn bn_shr(const bn *x, size_t s) {
    size_t w = s / 32, b = s % 32;
    if (w >= x->len) return bn_make(1);
    size_t L = x->len - w;
    bn r = bn_make(L);
    for (size_t i = 0; i < L; i++) {
        uint64_t v = x->d[i + w];
        if (i + w + 1 < x->len) v |= (uint64_t)x->d[i + w + 1] << 32;
        r.d[i] = (uint32_t)(v >> b);
    }
    r.len = L;
    bn_trim(&r);
    return r;
}
static bn bn_add(const bn *x, const bn *y) {
    if (x->len < y->len) { const bn *t = x; x = y; y = t; }
    bn r = bn_make(x->len + 1);
    uint64_t c = 0;
    for (size_t i = 0; i < x->len; i++) {
        c += x->d[i];
        if (i < y->len) c += y->d[i];
        r.d[i] = (uint32_t)c;
        c >>= 32;
    }
    r.d[x->len] = (uint32_t)c;
    r.len = x->len + 1;
    bn_trim(&r);
    return r;
}
/* acc += t, acc must have enough capacity (zeroed above len) */
static void bn_addin(bn *acc, const bn *t) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < t->len; i++) {
        c += (uint64_t)acc->d[i] + t->d[i];
        acc->d[i] = (uint32_t)c;
        c >>= 32;
    }
    for (; c; i++) {
        c += acc->d[i];
        acc->d[i] = (uint32_t)c;
        c >>= 32;
    }
    if (i > acc->len) acc->len = i;
    bn_trim(acc);
}
/* x - y, requires x >= y */
static bn bn_sub(const bn *x, const bn *y) {
    bn r = bn_make(x->len);
    uint64_t br = 0;
    for (size_t i = 0; i < x->len; i++) {
        uint64_t yi = i < y->len ? y->d[i] : 0;
        uint64_t v = (uint64_t)x->d[i] - yi - br;
        r.d[i] = (uint32_t)v;
        br = (v >> 63) & 1;
    }
    r.len = x->len;
    bn_trim(&r);
    return r;
}
static bn bn_mul(const bn *x, const bn *y) {
    if (!x->len || !y->len) return bn_make(1);
    bn r = bn_make(x->len + y->len);
    for (size_t i = 0; i < x->len; i++) {
        uint64_t xi = x->d[i], c = 0;
        if (!xi) continue;
        for (size_t j = 0; j < y->len; j++) {
            uint64_t t = xi * y->d[j] + r.d[i + j] + c;
            r.d[i + j] = (uint32_t)t;
            c = t >> 32;
        }
        r.d[i + y->len] = (uint32_t)c;
    }
    r.len = x->len + y->len;
    bn_trim(&r);
    return r;
}
/* dst = src / d (dst may equal src; dst capacity >= src->len) */
static void bn_divsmall(bn *dst, const bn *src, uint32_t d) {
    uint64_t m = UINT64_MAX / d, r = 0;
    size_t L = src->len;
    for (size_t i = L; i-- > 0;) {
        uint64_t x = (r << 32) | src->d[i];
        uint64_t q = (uint64_t)(((unsigned __int128)x * m) >> 64);
        r = x - q * d;
        while (r >= d) { q++; r -= d; }
        dst->d[i] = (uint32_t)q;
    }
    dst->len = L;
    bn_trim(dst);
}
/* floor(u / v), v > 0 (Knuth algorithm D) */
static bn bn_div(const bn *u, const bn *v) {
    if (bn_cmp(u, v) < 0) return bn_make(1);
    size_t m = u->len, n = v->len;
    if (n == 1) {
        bn q = bn_make(m);
        bn_divsmall(&q, u, v->d[0]);
        return q;
    }
    int s = 0;
    uint32_t top = v->d[n - 1];
    while (!(top & 0x80000000u)) { top <<= 1; s++; }
    uint32_t *vn = (uint32_t *)malloc(n * sizeof(uint32_t));
    uint32_t *un = (uint32_t *)malloc((m + 1) * sizeof(uint32_t));
    for (size_t i = n - 1; i > 0; i--)
        vn[i] = (uint32_t)(((((uint64_t)v->d[i]) << 32) | v->d[i - 1]) >> (32 - s));
    vn[0] = v->d[0] << s;
    un[m] = (uint32_t)(((uint64_t)u->d[m - 1]) >> (32 - s));
    for (size_t i = m - 1; i > 0; i--)
        un[i] = (uint32_t)(((((uint64_t)u->d[i]) << 32) | u->d[i - 1]) >> (32 - s));
    un[0] = u->d[0] << s;
    bn q = bn_make(m - n + 1);
    const uint64_t B = (uint64_t)1 << 32;
    for (size_t j = m - n + 1; j-- > 0;) {
        uint64_t num = ((uint64_t)un[j + n] << 32) | un[j + n - 1];
        uint64_t qhat = num / vn[n - 1], rhat = num % vn[n - 1];
        while (qhat >= B || qhat * vn[n - 2] > ((rhat << 32) | un[j + n - 2])) {
            qhat--;
            rhat += vn[n - 1];
            if (rhat >= B) break;
        }
        int64_t k = 0, t;
        for (size_t i = 0; i < n; i++) {
            uint64_t p = qhat * vn[i];
            t = (int64_t)un[i + j] - k - (int64_t)(p & 0xFFFFFFFFu);
            un[i + j] = (uint32_t)t;
            k = (int64_t)(p >> 32) - (t >> 32);
        }
        t = (int64_t)un[j + n] - k;
        un[j + n] = (uint32_t)t;
        if (t < 0) {
            qhat--;
            uint64_t c = 0;
            for (size_t i = 0; i < n; i++) {
                c += (uint64_t)un[i + j] + vn[i];
                un[i + j] = (uint32_t)c;
                c >>= 32;
            }
            un[j + n] += (uint32_t)c;
        }
        q.d[j] = (uint32_t)qhat;
    }
    q.len = m - n + 1;
    bn_trim(&q);
    free(vn);
    free(un);
    return q;
}
static uint64_t isqrt64(uint64_t x) {
    uint64_t r = 0, bit = (uint64_t)1 << 62;
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) { x -= r + bit; r = (r >> 1) + bit; }
        else r >>= 1;
        bit >>= 2;
    }
    return r;
}
static bn bn_isqrt(const bn *m) {
    size_t L = bn_bits(m);
    if (L <= 64) {
        uint64_t v = m->len ? m->d[0] : 0;
        if (m->len > 1) v |= (uint64_t)m->d[1] << 32;
        return bn_u64(isqrt64(v));
    }
    size_t k = L / 4;
    bn hi = bn_shr(m, 2 * k);
    bn s = bn_isqrt(&hi);
    bn one = bn_u64(1);
    bn s1 = bn_add(&s, &one);
    bn x = bn_shl(&s1, k);
    bn_free(&hi); bn_free(&s); bn_free(&one); bn_free(&s1);
    for (;;) {
        bn q = bn_div(m, &x);
        bn sm = bn_add(&x, &q);
        bn y = bn_shr(&sm, 1);
        bn_free(&q); bn_free(&sm);
        if (bn_cmp(&y, &x) >= 0) { bn_free(&y); break; }
        bn_free(&x);
        x = y;
    }
    return x;
}
/* atan(1/x) * 2^Q */
static bn atan_inv(uint32_t x, size_t Q) {
    size_t cap = Q / 32 + 3;
    bn one = bn_u64(1);
    bn term = bn_shl(&one, Q);
    bn_free(&one);
    bn_divsmall(&term, &term, x);
    bn pos = bn_make(cap), neg = bn_make(cap), t = bn_make(cap);
    uint32_t x2 = x * x;
    bn_addin(&pos, &term);
    for (uint32_t k = 1; term.len; k++) {
        bn_divsmall(&term, &term, x2);
        bn_divsmall(&t, &term, 2 * k + 1);
        bn_addin((k & 1) ? &neg : &pos, &t);
    }
    bn r = bn_sub(&pos, &neg);
    bn_free(&term); bn_free(&pos); bn_free(&neg); bn_free(&t);
    return r;
}
/* e^-1 * 2^Q */
static bn inv_e(size_t Q) {
    size_t cap = Q / 32 + 3;
    bn one = bn_u64(1);
    bn term = bn_shl(&one, Q);
    bn_free(&one);
    bn pos = bn_make(cap), neg = bn_make(cap);
    bn_addin(&pos, &term);
    for (uint32_t k = 1; term.len; k++) {
        bn_divsmall(&term, &term, k);
        bn_addin((k & 1) ? &neg : &pos, &term);
    }
    bn r = bn_sub(&pos, &neg);
    bn_free(&term); bn_free(&pos); bn_free(&neg);
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    int hb = 63;
    while (!((n >> hb) & 1)) hb--;

    /* N = n^n exactly */
    bn nb = bn_u64(n);
    bn N = bn_u64(1);
    for (int b = hb; b >= 0; b--) {
        bn t = bn_mul(&N, &N);
        bn_free(&N); N = t;
        if ((n >> b) & 1) { t = bn_mul(&N, &nb); bn_free(&N); N = t; }
    }
    size_t Lb = bn_bits(&N);
    size_t Q = Lb + (size_t)n + (size_t)(n / 2) + 2 * (size_t)(hb + 1) + 128;

    /* F = e^-n * 2^Q */
    bn ie = inv_e(Q);
    bn F = bn_copy(&ie);
    for (int b = hb - 1; b >= 0; b--) {
        bn t = bn_mul(&F, &F);
        bn_free(&F); F = bn_shr(&t, Q); bn_free(&t);
        if ((n >> b) & 1) {
            t = bn_mul(&F, &ie);
            bn_free(&F); F = bn_shr(&t, Q); bn_free(&t);
        }
    }

    /* S = sqrt(2*Pi*n) * 2^Q */
    bn a5 = atan_inv(5, Q), a239 = atan_inv(239, Q);
    bn p1 = bn_shl(&a5, 4), p2 = bn_shl(&a239, 2);
    bn pi = bn_sub(&p1, &p2);
    bn twon = bn_u64((uint64_t)n * 2);
    bn M0 = bn_mul(&pi, &twon);
    bn M = bn_shl(&M0, Q);
    bn S = bn_isqrt(&M);

    /* round(N * S * F / 2^(2Q)) */
    bn T0 = bn_mul(&N, &S);
    bn T = bn_mul(&T0, &F);
    bn one = bn_u64(1);
    bn half = bn_shl(&one, 2 * Q - 1);
    bn T1 = bn_add(&T, &half);
    bn R = bn_shr(&T1, 2 * Q);

    seq_t r = 0;
    for (size_t i = R.len; i-- > 0;) {
        r = (seq_t)(r * (idx_t)4294967296ULL);
        r = (seq_t)(r + (idx_t)R.d[i]);
    }

    bn_free(&nb); bn_free(&N); bn_free(&ie); bn_free(&F);
    bn_free(&a5); bn_free(&a239); bn_free(&p1); bn_free(&p2); bn_free(&pi);
    bn_free(&twon); bn_free(&M0); bn_free(&M); bn_free(&S);
    bn_free(&T0); bn_free(&T); bn_free(&one); bn_free(&half); bn_free(&T1); bn_free(&R);
    return r;
}
