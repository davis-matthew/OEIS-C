/*
 * A002831  Number of 3-edge-colored connected trivalent graphs with 2n nodes.
 * https://oeis.org/A002831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Sum_{k>=1} mobius(k)*log(G(x^k))/k, G = g.f. of A002830
 *            (Burnside over centralizers). log G(x) = Sum_{M>=1} T(M) x^M / M
 *            with T(M) = Sum_{k|M} S_k(M/k), where S_k(m) = R_k(m)/(k^m m!),
 *            R_k(m) = k*m*A_k(m) - Sum_{j=1..m-1} C(m,j) R_k(j) A_k(m-j),
 *            A_k(m) = c_k(m)^3, c_k(0)=1, c_k(1)=[k even],...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   q31_mu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/q31_mu.h"

typedef struct { uint32_t *d; size_t n, cap; } q31_t;
typedef struct { q31_t *c, *A, *R; size_t len, cap; } q31_ser;

static void q31_reserve(q31_t *x, size_t c) {
    if (c > x->cap) {
        size_t nc = c + c / 2 + 4;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}
static void q31_trim(q31_t *x) { while (x->n && x->d[x->n - 1] == 0) x->n--; }
static void q31_set_u(q31_t *x, uint64_t v) {
    q31_reserve(x, 2);
    x->n = 0;
    while (v) { x->d[x->n++] = (uint32_t)v; v >>= 32; }
}
static void q31_copy(q31_t *x, const q31_t *y) {
    q31_reserve(x, y->n + 1);
    if (y->n) memcpy(x->d, y->d, y->n * sizeof(uint32_t));
    x->n = y->n;
}
static void q31_free(q31_t *x) { free(x->d); x->d = NULL; x->n = x->cap = 0; }
static void q31_muls(q31_t *x, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    if (m == 0) { x->n = 0; return; }
    for (i = 0; i < x->n; i++) {
        uint64_t t = (uint64_t)x->d[i] * m + c;
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) { q31_reserve(x, x->n + 1); x->d[x->n++] = (uint32_t)c; }
}
static void q31_divs(q31_t *x, uint32_t m) {
    uint64_t r = 0;
    size_t i;
    for (i = x->n; i-- > 0;) {
        uint64_t cur = (r << 32) | x->d[i];
        x->d[i] = (uint32_t)(cur / m);
        r = cur % m;
    }
    q31_trim(x);
}
static void q31_add(q31_t *x, const q31_t *y) {
    size_t L = x->n > y->n ? x->n : y->n, i;
    uint64_t carry = 0;
    q31_reserve(x, L + 1);
    for (i = x->n; i < L; i++) x->d[i] = 0;
    for (i = 0; i < L; i++) {
        uint64_t s = (uint64_t)x->d[i] + (i < y->n ? y->d[i] : 0) + carry;
        x->d[i] = (uint32_t)s;
        carry = s >> 32;
    }
    if (carry) { x->d[L] = (uint32_t)carry; x->n = L + 1; }
    else x->n = L;
}
static void q31_sub(q31_t *x, const q31_t *y) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < x->n; i++) {
        int64_t t = (int64_t)x->d[i] - (int64_t)(i < y->n ? y->d[i] : 0) - borrow;
        if (t < 0) { t += (int64_t)1 << 32; borrow = 1; }
        else borrow = 0;
        x->d[i] = (uint32_t)t;
    }
    q31_trim(x);
}
static void q31_mul(q31_t *out, const q31_t *a, const q31_t *b) {
    size_t i, j;
    if (a->n == 0 || b->n == 0) { out->n = 0; return; }
    q31_reserve(out, a->n + b->n);
    memset(out->d, 0, (a->n + b->n) * sizeof(uint32_t));
    for (i = 0; i < a->n; i++) {
        uint64_t carry = 0;
        for (j = 0; j < b->n; j++) {
            uint64_t t = (uint64_t)a->d[i] * b->d[j] + out->d[i + j] + carry;
            out->d[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        out->d[i + b->n] = (uint32_t)carry;
    }
    out->n = a->n + b->n;
    q31_trim(out);
}

static q31_ser *g_ser = NULL;
static size_t g_nser = 0;
static q31_t *g_T = NULL;
static idx_t g_N = 0;

static void q31_push(q31_ser *s, idx_t k) {
    size_t m = s->len;
    q31_t *c, *A, *R;
    if (m == s->cap) {
        size_t nc = s->cap * 2 + 4;
        s->c = (q31_t *)realloc(s->c, nc * sizeof(q31_t));
        s->A = (q31_t *)realloc(s->A, nc * sizeof(q31_t));
        s->R = (q31_t *)realloc(s->R, nc * sizeof(q31_t));
        memset(s->c + s->cap, 0, (nc - s->cap) * sizeof(q31_t));
        memset(s->A + s->cap, 0, (nc - s->cap) * sizeof(q31_t));
        memset(s->R + s->cap, 0, (nc - s->cap) * sizeof(q31_t));
        s->cap = nc;
    }
    c = &s->c[m]; A = &s->A[m]; R = &s->R[m];
    if (m == 0) q31_set_u(c, 1);
    else if (m == 1) q31_set_u(c, (k % 2 == 0) ? 1 : 0);
    else {
        q31_copy(c, &s->c[m - 2]);
        q31_muls(c, (uint32_t)(m - 1));
        q31_muls(c, (uint32_t)k);
        if (k % 2 == 0) q31_add(c, &s->c[m - 1]);
    }
    {
        q31_t t = {NULL, 0, 0};
        q31_mul(&t, c, c);
        q31_mul(A, &t, c);
        q31_free(&t);
    }
    if (m == 0) q31_set_u(R, 0);
    else {
        q31_t sb = {NULL, 0, 0}, bin = {NULL, 0, 0}, t1 = {NULL, 0, 0}, t2 = {NULL, 0, 0};
        size_t j;
        q31_copy(R, A);
        q31_muls(R, (uint32_t)k);
        q31_muls(R, (uint32_t)m);
        q31_set_u(&sb, 0);
        q31_set_u(&bin, m);
        for (j = 1; j < m; j++) {
            if (s->R[j].n && s->A[m - j].n) {
                q31_mul(&t1, &s->R[j], &s->A[m - j]);
                q31_mul(&t2, &t1, &bin);
                q31_add(&sb, &t2);
            }
            q31_muls(&bin, (uint32_t)(m - j));
            q31_divs(&bin, (uint32_t)(j + 1));
        }
        q31_sub(R, &sb);
        q31_free(&sb); q31_free(&bin); q31_free(&t1); q31_free(&t2);
    }
    s->len = m + 1;
}

static void q31_extend(idx_t newN) {
    idx_t M, k;
    if (newN <= g_N) return;
    if (newN + 1 > g_nser) {
        g_ser = (q31_ser *)realloc(g_ser, (size_t)(newN + 1) * sizeof(q31_ser));
        memset(g_ser + g_nser, 0, (size_t)(newN + 1 - g_nser) * sizeof(q31_ser));
        g_T = (q31_t *)realloc(g_T, (size_t)(newN + 1) * sizeof(q31_t));
        memset(g_T + g_nser, 0, (size_t)(newN + 1 - g_nser) * sizeof(q31_t));
        g_nser = (size_t)(newN + 1);
    }
    for (M = g_N + 1; M <= newN; M++) {
        q31_set_u(&g_T[M], 0);
        for (k = 1; k <= M; k++) {
            idx_t m, i;
            q31_ser *s;
            q31_t S = {NULL, 0, 0};
            if (M % k) continue;
            m = M / k;
            s = &g_ser[k];
            while (s->len <= m) q31_push(s, k);
            q31_copy(&S, &s->R[m]);
            for (i = 0; i < m; i++) q31_divs(&S, (uint32_t)k);
            for (i = 2; i <= m; i++) q31_divs(&S, (uint32_t)i);
            q31_add(&g_T[M], &S);
            q31_free(&S);
        }
    }
    g_N = newN;
}

seq_t a(idx_t n) {
    idx_t N, j;
    q31_t pos = {NULL, 0, 0}, neg = {NULL, 0, 0};
    seq_t result = 0, base = 1;
    size_t i;
    if (n == 0) return 0;
    N = 2 * n;
    q31_extend(N);
    q31_set_u(&pos, 0);
    q31_set_u(&neg, 0);
    for (j = 1; j <= N; j++) {
        int mu;
        if (N % j) continue;
        mu = q31_mu(j);
        if (mu > 0) q31_add(&pos, &g_T[N / j]);
        else if (mu < 0) q31_add(&neg, &g_T[N / j]);
    }
    q31_sub(&pos, &neg);
    q31_divs(&pos, (uint32_t)N);
    for (i = 0; i < pos.n; i++) {
        if (base == 0) break;
        result += (seq_t)pos.d[i] * base;
        base = base * (seq_t)65536;
        base = base * (seq_t)65536;
    }
    q31_free(&pos);
    q31_free(&neg);
    return result;
}
