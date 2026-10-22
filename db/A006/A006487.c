/*
 * A006487  Denominators of greedy Egyptian fraction for square root of 2.
 * https://oeis.org/A006487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(1/(sqrt(2) - Sum_{j=0..n-1} 1/a(j))), a(0) = 1.
 *            With P/Q = Sum_{j=0..n-1} 1/a(j), it is computed exactly as a(n)
 *            = floor((P*Q + isqrt(2*Q^4)) / (2*Q^2 - P^2)) + 1
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   a_addbit, a_cmp, a_shr1, a_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/a_addbit.h"
#include "../utils/a_cmp.h"
#include "../utils/a_shr1.h"
#include "../utils/a_sub.h"

typedef struct { uint32_t *d; size_t n; } big_t;

static big_t b_alloc(size_t cap) {
    big_t r;
    r.d = (uint32_t *)calloc(cap ? cap : 1, sizeof(uint32_t));
    r.n = cap;
    return r;
}

static void b_norm(big_t *x) {
    while (x->n && x->d[x->n - 1] == 0) x->n--;
}

static void b_free(big_t *x) {
    free(x->d);
    x->d = NULL;
    x->n = 0;
}

static big_t b_small(uint32_t v) {
    big_t r = b_alloc(1);
    r.d[0] = v;
    b_norm(&r);
    return r;
}

static big_t b_mul(big_t a, big_t b) {
    big_t r = b_alloc(a.n + b.n);
    size_t i, j;
    for (i = 0; i < a.n; i++) {
        uint64_t carry = 0, ai = a.d[i];
        if (!ai) continue;
        for (j = 0; j < b.n; j++) {
            uint64_t t = ai * b.d[j] + r.d[i + j] + carry;
            r.d[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        r.d[i + b.n] = (uint32_t)carry;
    }
    b_norm(&r);
    return r;
}

static big_t b_add(big_t a, big_t b) {
    size_t L = (a.n > b.n ? a.n : b.n) + 1, i;
    big_t r = b_alloc(L);
    uint64_t carry = 0;
    for (i = 0; i < L; i++) {
        uint64_t t = carry;
        if (i < a.n) t += a.d[i];
        if (i < b.n) t += b.d[i];
        r.d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    b_norm(&r);
    return r;
}

/* requires a >= b */
static big_t b_sub(big_t a, big_t b) {
    big_t r = b_alloc(a.n);
    uint64_t borrow = 0;
    size_t i;
    for (i = 0; i < a.n; i++) {
        uint64_t t = (uint64_t)a.d[i] - (i < b.n ? b.d[i] : 0) - borrow;
        r.d[i] = (uint32_t)t;
        borrow = (t >> 32) & 1;
    }
    b_norm(&r);
    return r;
}

static size_t b_bits(big_t a) {
    size_t bits;
    uint32_t top;
    if (!a.n) return 0;
    bits = (a.n - 1) * 32;
    top = a.d[a.n - 1];
    while (top) { bits++; top >>= 1; }
    return bits;
}

static int b_cmp(big_t a, big_t b) {
    if (a.n != b.n) return a.n < b.n ? -1 : 1;
    return a_cmp(a.d, b.d, a.n);
}

static big_t b_isqrt(big_t x) {
    size_t L = x.n, p;
    uint32_t *rem, *res, *t;
    big_t r;
    if (!L) return b_alloc(0);
    rem = (uint32_t *)calloc(L, sizeof(uint32_t));
    res = (uint32_t *)calloc(L, sizeof(uint32_t));
    t = (uint32_t *)calloc(L, sizeof(uint32_t));
    memcpy(rem, x.d, L * sizeof(uint32_t));
    p = (b_bits(x) - 1) & ~(size_t)1;
    for (;;) {
        memcpy(t, res, L * sizeof(uint32_t));
        a_addbit(t, p, L);
        if (a_cmp(rem, t, L) >= 0) {
            a_sub(rem, t, L);
            a_shr1(res, L);
            a_addbit(res, p, L);
        } else {
            a_shr1(res, L);
        }
        if (p < 2) break;
        p -= 2;
    }
    r.d = res;
    r.n = L;
    b_norm(&r);
    free(rem);
    free(t);
    return r;
}

static big_t b_div(big_t N, big_t D) {
    size_t s, L, ls, bs, i;
    uint32_t *rem, *dd;
    big_t q;
    if (b_cmp(N, D) < 0) return b_alloc(0);
    s = b_bits(N) - b_bits(D);
    L = N.n + 1;
    rem = (uint32_t *)calloc(L, sizeof(uint32_t));
    dd = (uint32_t *)calloc(L, sizeof(uint32_t));
    memcpy(rem, N.d, N.n * sizeof(uint32_t));
    ls = s / 32;
    bs = s % 32;
    for (i = 0; i < D.n; i++) {
        dd[i + ls] |= (uint32_t)(D.d[i] << bs);
        if (bs) dd[i + ls + 1] |= D.d[i] >> (32 - bs);
    }
    q = b_alloc(s / 32 + 1);
    for (i = s + 1; i-- > 0;) {
        if (a_cmp(rem, dd, L) >= 0) {
            a_sub(rem, dd, L);
            q.d[i / 32] |= (uint32_t)1 << (i % 32);
        }
        a_shr1(dd, L);
    }
    free(rem);
    free(dd);
    b_norm(&q);
    return q;
}

static seq_t b_to_seq(big_t x) {
    seq_t v = 0;
    size_t i;
    for (i = x.n; i-- > 0;) {
        v = v * (seq_t)65536 + (seq_t)(int)(x.d[i] >> 16);
        v = v * (seq_t)65536 + (seq_t)(int)(x.d[i] & 0xFFFFu);
    }
    return v;
}

seq_t a(idx_t n) {
    static bool inited = false;
    static big_t P, Q;
    static seq_t *cache = NULL;
    static idx_t have = 0, cap = 0;

    if (!inited) {
        P = b_alloc(0);
        Q = b_small(1);
        inited = true;
    }
    while (have <= n) {
        big_t Q2 = b_mul(Q, Q);
        big_t P2 = b_mul(P, P);
        big_t Q2x2 = b_add(Q2, Q2);
        big_t D = b_sub(Q2x2, P2);
        big_t Q4 = b_mul(Q2, Q2);
        big_t T = b_add(Q4, Q4);
        big_t F = b_isqrt(T);
        big_t PQ = b_mul(P, Q);
        big_t N = b_add(PQ, F);
        big_t qt = b_div(N, D);
        big_t one = b_small(1);
        big_t A = b_add(qt, one);
        big_t PA, nP, nQ;

        if (have == cap) {
            idx_t ncap = cap ? cap * 2 : 16;
            cache = (seq_t *)realloc(cache, ncap * sizeof(seq_t));
            cap = ncap;
        }
        cache[have++] = b_to_seq(A);

        PA = b_mul(P, A);
        nP = b_add(PA, Q);
        nQ = b_mul(Q, A);

        b_free(&Q2); b_free(&P2); b_free(&Q2x2); b_free(&D);
        b_free(&Q4); b_free(&T); b_free(&F); b_free(&PQ);
        b_free(&N); b_free(&qt); b_free(&one); b_free(&A);
        b_free(&PA); b_free(&P); b_free(&Q);
        P = nP;
        Q = nQ;
    }
    return cache[n];
}
