/*
 * A002209  Denominators of coefficients for numerical integration.
 * https://oeis.org/A002209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, nice
 * Formula:   a(n) = denominator(((-1)^n/n!)*Sum_{k=0..n}
 *            Stirling1(n+1,k+1)/(k+1)), i.e. v(n) = (Sum_{k=0..n}
 *            (-1)^k*|Stirling1(n+1,k+1)|*L/(k+1)) / (n!*L), L = lcm(1..n+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t len, cap; } g9_big;

static void g9_reserve(g9_big *x, size_t need) {
    if (need > x->cap) {
        size_t c = x->cap ? x->cap : 4;
        while (c < need) c *= 2;
        x->d = (uint32_t *)realloc(x->d, c * sizeof(uint32_t));
        x->cap = c;
    }
}

static void g9_norm(g9_big *x) {
    while (x->len > 0 && x->d[x->len - 1] == 0) x->len--;
}

/* x = x*m + y */
static void g9_muladd_small(g9_big *x, uint32_t m, const g9_big *y) {
    size_t L = x->len > y->len ? x->len : y->len;
    g9_reserve(x, L + 1);
    uint64_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t xi = i < x->len ? x->d[i] : 0;
        uint64_t yi = i < y->len ? y->d[i] : 0;
        uint64_t t = xi * m + yi + carry;
        x->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    x->d[L] = (uint32_t)carry;
    x->len = L + 1;
    g9_norm(x);
}

static void g9_mul_small(g9_big *x, uint32_t m) {
    g9_reserve(x, x->len + 1);
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; i++) {
        uint64_t t = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    x->d[x->len] = (uint32_t)carry;
    x->len++;
    g9_norm(x);
}

static void g9_div_small(g9_big *dst, const g9_big *src, uint32_t q) {
    g9_reserve(dst, src->len + 1);
    uint64_t r = 0;
    for (size_t i = src->len; i-- > 0;) {
        uint64_t cur = (r << 32) | src->d[i];
        dst->d[i] = (uint32_t)(cur / q);
        r = cur % q;
    }
    dst->len = src->len;
    g9_norm(dst);
}

static uint32_t g9_mod_small(const g9_big *x, uint32_t q) {
    uint64_t r = 0;
    for (size_t i = x->len; i-- > 0;) r = ((r << 32) | x->d[i]) % q;
    return (uint32_t)r;
}

/* acc += x*y */
static void g9_addmul(g9_big *acc, const g9_big *x, const g9_big *y) {
    if (x->len == 0 || y->len == 0) return;
    size_t need = x->len + y->len;
    if (acc->len > need) need = acc->len;
    need++;
    g9_reserve(acc, need);
    for (size_t i = acc->len; i < need; i++) acc->d[i] = 0;
    for (size_t i = 0; i < x->len; i++) {
        uint64_t carry = 0, xi = x->d[i];
        size_t j;
        for (j = 0; j < y->len; j++) {
            uint64_t t = acc->d[i + j] + xi * y->d[j] + carry;
            acc->d[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        for (size_t k = i + j; carry && k < need; k++) {
            uint64_t t = acc->d[k] + carry;
            acc->d[k] = (uint32_t)t;
            carry = t >> 32;
        }
    }
    acc->len = need;
    g9_norm(acc);
}

/* a -= b, requires a >= b */
static void g9_sub(g9_big *a, const g9_big *b) {
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; i++) {
        uint64_t bi = i < b->len ? b->d[i] : 0;
        uint64_t s = bi + borrow;
        uint64_t ai = a->d[i];
        if (ai >= s) { a->d[i] = (uint32_t)(ai - s); borrow = 0; }
        else { a->d[i] = (uint32_t)((ai + ((uint64_t)1 << 32)) - s); borrow = 1; }
    }
    g9_norm(a);
}

seq_t a(idx_t n) {
    idx_t N1 = n + 1;
    /* unsigned Stirling numbers of the first kind, row n+1 */
    g9_big *c = (g9_big *)calloc((size_t)N1 + 1, sizeof(g9_big));
    g9_reserve(&c[1], 1);
    c[1].d[0] = 1;
    c[1].len = 1;
    for (idx_t m = 1; m <= n; m++)
        for (idx_t k = m + 1; k >= 1; k--)
            g9_muladd_small(&c[k], (uint32_t)m, &c[k - 1]);

    /* primes up to n+1 and L = lcm(1..n+1) */
    char *comp = (char *)calloc((size_t)N1 + 1, 1);
    g9_big L = {0, 0, 0};
    g9_reserve(&L, 1);
    L.d[0] = 1;
    L.len = 1;
    for (idx_t p = 2; p <= N1; p++) {
        if (comp[p]) continue;
        for (idx_t q = p * p; q <= N1; q += p) comp[q] = 1;
        idx_t pe = p;
        while (pe <= N1 / p) pe *= p;
        g9_mul_small(&L, (uint32_t)pe);
    }

    g9_big P = {0, 0, 0}, Q = {0, 0, 0}, tmp = {0, 0, 0};
    for (idx_t k = 0; k <= n; k++) {
        g9_div_small(&tmp, &L, (uint32_t)(k + 1));
        if (k % 2 == 0) g9_addmul(&P, &c[k + 1], &tmp);
        else g9_addmul(&Q, &c[k + 1], &tmp);
    }
    g9_sub(&P, &Q); /* P = v(n) * n! * L */

    seq_t res = 1;
    for (idx_t p = 2; p <= N1; p++) {
        if (comp[p]) continue;
        idx_t e = 0;
        for (idx_t q = n / p; q > 0; q /= p) e += q;
        idx_t pe = p;
        e++;
        while (pe <= N1 / p) { pe *= p; e++; }
        idx_t cnt = 0;
        while (cnt < e && g9_mod_small(&P, (uint32_t)p) == 0) {
            g9_div_small(&P, &P, (uint32_t)p);
            cnt++;
        }
        for (idx_t i = cnt; i < e; i++) res *= (seq_t)p;
    }

    for (idx_t k = 0; k <= N1; k++) free(c[k].d);
    free(c);
    free(comp);
    free(L.d);
    free(P.d);
    free(Q.d);
    free(tmp.d);
    return res;
}
