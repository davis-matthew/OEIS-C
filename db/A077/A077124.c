/*
 * A077124  Decimal expansion of the constant c such that A003096(n-1) =
 *          ceiling(c^(2^n)).
 * https://oeis.org/A077124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   c = lim_{n->inf} A003096(n-1)^(1/2^n), where A003096(k) =
 *            A003096(k-1)^2 - 1, A003096(0) = 2; a(n) = n-th decimal digit of
 *            c = 1.2955535361865... Computed by taking 2^N-th root of
 *            A003096(N-1) via repeated fixed-point integer square roots (own
 *            bignum), digits cached
 * Range:     u8 n <= 100009, u16 n <= 100009, u32 n <= 100009,
 *            u64 n <= 100009, u128 n <= 100009, big any n
 * Validated: standard, pass, 121 terms through n = 8830 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

typedef struct { uint32_t *d; size_t n; } A077124_BN;

static A077124_BN A077124_alloc(size_t cap) {
    A077124_BN r;
    r.d = (uint32_t *)calloc(cap ? cap : 1, sizeof(uint32_t));
    r.n = 0;
    return r;
}
static void A077124_norm(A077124_BN *a) { while (a->n && a->d[a->n - 1] == 0) a->n--; }
static void A077124_free(A077124_BN *a) { free(a->d); a->d = NULL; a->n = 0; }
static A077124_BN A077124_u64(uint64_t v) {
    A077124_BN r = A077124_alloc(2);
    r.d[0] = (uint32_t)v; r.d[1] = (uint32_t)(v >> 32); r.n = 2;
    A077124_norm(&r);
    return r;
}
static int A077124_cmp(const A077124_BN *a, const A077124_BN *b) {
    if (a->n != b->n) return a->n < b->n ? -1 : 1;
    for (size_t i = a->n; i-- > 0;) {
        if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    }
    return 0;
}
static A077124_BN A077124_add(const A077124_BN *a, const A077124_BN *b) {
    size_t m = a->n > b->n ? a->n : b->n;
    A077124_BN r = A077124_alloc(m + 1);
    uint64_t c = 0;
    for (size_t i = 0; i < m; i++) {
        uint64_t t = c;
        if (i < a->n) t += a->d[i];
        if (i < b->n) t += b->d[i];
        r.d[i] = (uint32_t)t;
        c = t >> 32;
    }
    r.d[m] = (uint32_t)c;
    r.n = m + 1;
    A077124_norm(&r);
    return r;
}
static A077124_BN A077124_sub(const A077124_BN *a, const A077124_BN *b) {
    A077124_BN r = A077124_alloc(a->n);
    int64_t br = 0;
    for (size_t i = 0; i < a->n; i++) {
        int64_t t = (int64_t)a->d[i] - br - (i < b->n ? (int64_t)b->d[i] : 0);
        if (t < 0) { t += ((int64_t)1 << 32); br = 1; } else br = 0;
        r.d[i] = (uint32_t)t;
    }
    r.n = a->n;
    A077124_norm(&r);
    return r;
}
static A077124_BN A077124_mul(const A077124_BN *a, const A077124_BN *b) {
    if (a->n == 0 || b->n == 0) return A077124_alloc(1);
    A077124_BN r = A077124_alloc(a->n + b->n);
    for (size_t i = 0; i < a->n; i++) {
        uint64_t carry = 0;
        uint64_t ai = a->d[i];
        for (size_t j = 0; j < b->n; j++) {
            uint64_t t = ai * b->d[j] + r.d[i + j] + carry;
            r.d[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        r.d[i + b->n] = (uint32_t)carry;
    }
    r.n = a->n + b->n;
    A077124_norm(&r);
    return r;
}
static A077124_BN A077124_shl(const A077124_BN *a, size_t bits) {
    size_t limbs = bits / 32;
    unsigned sh = (unsigned)(bits % 32);
    A077124_BN r = A077124_alloc(a->n + limbs + 1);
    if (a->n == 0) return r;
    for (size_t i = 0; i < a->n; i++) {
        r.d[i + limbs] |= a->d[i] << sh;
        if (sh) r.d[i + limbs + 1] |= a->d[i] >> (32 - sh);
    }
    r.n = a->n + limbs + 1;
    A077124_norm(&r);
    return r;
}
static A077124_BN A077124_shr(const A077124_BN *a, size_t bits) {
    size_t limbs = bits / 32;
    unsigned sh = (unsigned)(bits % 32);
    if (limbs >= a->n) return A077124_alloc(1);
    A077124_BN r = A077124_alloc(a->n - limbs);
    for (size_t i = limbs; i < a->n; i++) {
        uint32_t v = a->d[i] >> sh;
        if (sh && i + 1 < a->n) v |= a->d[i + 1] << (32 - sh);
        r.d[i - limbs] = v;
    }
    r.n = a->n - limbs;
    A077124_norm(&r);
    return r;
}
static size_t A077124_bits(const A077124_BN *a) {
    if (a->n == 0) return 0;
    uint32_t t = a->d[a->n - 1];
    size_t b = 0;
    while (t) { b++; t >>= 1; }
    return 32 * (a->n - 1) + b;
}

/* floor(u / v), v nonzero (Knuth algorithm D) */
static A077124_BN A077124_divq(const A077124_BN *u, const A077124_BN *v) {
    size_t m = u->n, n = v->n;
    if (m < n || A077124_cmp(u, v) < 0) return A077124_alloc(1);
    A077124_BN q = A077124_alloc(m - n + 1);
    if (n == 1) {
        uint64_t r = 0;
        for (size_t i = m; i-- > 0;) {
            uint64_t cur = (r << 32) | u->d[i];
            q.d[i] = (uint32_t)(cur / v->d[0]);
            r = cur % v->d[0];
        }
        q.n = m;
        A077124_norm(&q);
        return q;
    }
    unsigned s = 0;
    uint32_t top = v->d[n - 1];
    while (!(top & 0x80000000u)) { top <<= 1; s++; }
    uint32_t *vn = (uint32_t *)malloc(n * sizeof(uint32_t));
    uint32_t *un = (uint32_t *)malloc((m + 1) * sizeof(uint32_t));
    for (size_t i = n - 1; i > 0; i--)
        vn[i] = (v->d[i] << s) | (s ? (v->d[i - 1] >> (32 - s)) : 0);
    vn[0] = v->d[0] << s;
    un[m] = s ? (u->d[m - 1] >> (32 - s)) : 0;
    for (size_t i = m - 1; i > 0; i--)
        un[i] = (u->d[i] << s) | (s ? (u->d[i - 1] >> (32 - s)) : 0);
    un[0] = u->d[0] << s;
    const uint64_t B = (uint64_t)1 << 32;
    for (long long j = (long long)(m - n); j >= 0; j--) {
        uint64_t num = ((uint64_t)un[j + n] << 32) | un[j + n - 1];
        uint64_t qhat = num / vn[n - 1];
        uint64_t rhat = num - qhat * vn[n - 1];
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
        q.d[j] = (uint32_t)qhat;
        if (t < 0) {
            q.d[j]--;
            uint64_t c = 0;
            for (size_t i = 0; i < n; i++) {
                uint64_t tt = (uint64_t)un[i + j] + vn[i] + c;
                un[i + j] = (uint32_t)tt;
                c = tt >> 32;
            }
            un[j + n] += (uint32_t)c;
        }
    }
    free(vn);
    free(un);
    q.n = m - n + 1;
    A077124_norm(&q);
    return q;
}

/* approximate floor sqrt (exact for small inputs, within a couple of units otherwise) */
static A077124_BN A077124_isqrt(const A077124_BN *N) {
    if (N->n == 0) return A077124_alloc(1);
    if (N->n < 8) {
        A077124_BN one = A077124_u64(1);
        A077124_BN x = A077124_shl(&one, (A077124_bits(N) + 1) / 2);
        A077124_free(&one);
        for (;;) {
            A077124_BN q = A077124_divq(N, &x);
            A077124_BN s = A077124_add(&x, &q);
            A077124_BN y = A077124_shr(&s, 1);
            A077124_free(&q);
            A077124_free(&s);
            if (A077124_cmp(&y, &x) >= 0) { A077124_free(&y); break; }
            A077124_free(&x);
            x = y;
        }
        return x;
    }
    size_t h = N->n / 4 - 1;
    A077124_BN top = A077124_shr(N, 64 * h);
    A077124_BN rp = A077124_isqrt(&top);
    A077124_BN r0 = A077124_shl(&rp, 32 * h);
    A077124_free(&top);
    A077124_free(&rp);
    A077124_BN q = A077124_divq(N, &r0);
    A077124_BN s = A077124_add(&r0, &q);
    A077124_BN r = A077124_shr(&s, 1);
    A077124_free(&r0);
    A077124_free(&q);
    A077124_free(&s);
    return r;
}

static unsigned char *A077124_dig = NULL;
static size_t A077124_cnt = 0;

static void A077124_build(size_t D) {
    size_t Fl = (size_t)((double)(D + 25) * 3.3220 / 32.0) + 3;
    size_t F = 32 * Fl;
    A077124_BN b = A077124_u64(2);
    size_t k = 0;
    while (A077124_bits(&b) < F / 2 + 64) {
        A077124_BN sq = A077124_mul(&b, &b);
        A077124_BN one = A077124_u64(1);
        A077124_BN nb = A077124_sub(&sq, &one);
        A077124_free(&sq);
        A077124_free(&one);
        A077124_free(&b);
        b = nb;
        k++;
    }
    size_t steps = k + 1;
    A077124_BN S = A077124_shl(&b, F);
    A077124_free(&b);
    for (size_t i = 0; i < steps; i++) {
        A077124_BN T = A077124_shl(&S, F);
        A077124_free(&S);
        S = A077124_isqrt(&T);
        A077124_free(&T);
    }
    uint32_t *frac = (uint32_t *)calloc(Fl + 1, sizeof(uint32_t));
    for (size_t i = 0; i < Fl && i < S.n; i++) frac[i] = S.d[i];
    unsigned char *dg = (unsigned char *)malloc(D + 16);
    size_t pos = 0;
    dg[pos++] = 1;
    while (pos < D + 9) {
        uint64_t carry = 0;
        for (size_t i = 0; i < Fl; i++) {
            uint64_t t = (uint64_t)frac[i] * 1000000000u + carry;
            frac[i] = (uint32_t)t;
            carry = t >> 32;
        }
        for (int j = 8; j >= 0; j--) {
            if (pos + (size_t)j < D + 16) dg[pos + (size_t)j] = (unsigned char)(carry % 10);
            carry /= 10;
        }
        pos += 9;
    }
    free(frac);
    A077124_free(&S);
    free(A077124_dig);
    A077124_dig = dg;
    A077124_cnt = D;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if ((size_t)n > A077124_cnt) {
        size_t D = (size_t)n;
        if (D < 2 * A077124_cnt) D = 2 * A077124_cnt;
        if (D < 64) D = 64;
        A077124_build(D);
    }
    return (seq_t)A077124_dig[n - 1];
}
