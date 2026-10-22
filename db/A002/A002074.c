/*
 * A002074  Denominators of coefficients in an asymptotic expansion of the
 *          confluent hypergeometric function F(1-b; 2; 4b).
 * https://oeis.org/A002074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   by Lagrange inversion, (2n+1)*[x^(2n+1)] g(x) = [s^n]
 *            u(s)^(-(2n+1)/3), where u(s) = Sum_{j>=0} 3/(2j+3) s^j (so
 *            f(x)^3 = x^3 u(x^2)). a(n) = denominator of P_n, where P_0 = 1
 *            and, for k >= 1, P_k = -(1/k) * Sum_{j=1..k} (2(n-1)j +
 *            3k)/(2j+3) * P_{k-j} (power-of-series recurrence; exact
 *            rationals...
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   z74_addfac
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/z74_addfac.h"

typedef struct { uint32_t *d; size_t len, cap; } z74_bn;

static void z74_reserve(z74_bn *x, size_t c) {
    if (c > x->cap) {
        size_t nc = c * 2 + 4;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}

static void z74_trim(z74_bn *x) {
    while (x->len && x->d[x->len - 1] == 0) x->len--;
}

static void z74_copy(z74_bn *dst, const z74_bn *src) {
    z74_reserve(dst, src->len);
    if (src->len) memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
    dst->len = src->len;
}

static void z74_mulu(z74_bn *x, uint32_t m) {
    if (m == 0) { x->len = 0; return; }
    uint64_t c = 0;
    for (size_t i = 0; i < x->len; i++) {
        c += (uint64_t)x->d[i] * m;
        x->d[i] = (uint32_t)c;
        c >>= 32;
    }
    if (c) { z74_reserve(x, x->len + 1); x->d[x->len++] = (uint32_t)c; }
}

static uint32_t z74_modu(const z74_bn *x, uint32_t m) {
    uint64_t r = 0;
    for (size_t i = x->len; i-- > 0;) r = ((r << 32) | x->d[i]) % m;
    return (uint32_t)r;
}

static void z74_divu(z74_bn *x, uint32_t m) {
    uint64_t r = 0;
    for (size_t i = x->len; i-- > 0;) {
        uint64_t cur = (r << 32) | x->d[i];
        x->d[i] = (uint32_t)(cur / m);
        r = cur % m;
    }
    z74_trim(x);
}

static int z74_cmp(const z74_bn *a, const z74_bn *b) {
    if (a->len != b->len) return a->len < b->len ? -1 : 1;
    for (size_t i = a->len; i-- > 0;)
        if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    return 0;
}

static void z74_add(z74_bn *a, const z74_bn *b) {
    size_t L = (a->len > b->len ? a->len : b->len) + 1;
    z74_reserve(a, L);
    for (size_t i = a->len; i < L; i++) a->d[i] = 0;
    uint64_t c = 0;
    for (size_t i = 0; i < L; i++) {
        c += (uint64_t)a->d[i] + (i < b->len ? b->d[i] : 0);
        a->d[i] = (uint32_t)c;
        c >>= 32;
    }
    a->len = L;
    z74_trim(a);
}

/* a -= b, requires a >= b */
static void z74_sub(z74_bn *a, const z74_bn *b) {
    uint64_t br = 0;
    for (size_t i = 0; i < a->len; i++) {
        uint64_t s = (i < b->len ? (uint64_t)b->d[i] : 0) + br;
        if ((uint64_t)a->d[i] >= s) { a->d[i] = (uint32_t)((uint64_t)a->d[i] - s); br = 0; }
        else { a->d[i] = (uint32_t)((uint64_t)a->d[i] + 0x100000000ULL - s); br = 1; }
    }
    z74_trim(a);
}

static void z74_sadd(z74_bn *acc, int *sa, const z74_bn *t, int st, z74_bn *tmp) {
    if (t->len == 0) return;
    if (acc->len == 0) { z74_copy(acc, t); *sa = st; return; }
    if (*sa == st) { z74_add(acc, t); return; }
    if (z74_cmp(acc, t) >= 0) {
        z74_sub(acc, t);
        if (acc->len == 0) *sa = 1;
    } else {
        z74_bn s;
        z74_copy(tmp, t);
        z74_sub(tmp, acc);
        s = *acc; *acc = *tmp; *tmp = s;
        *sa = st;
    }
}

static void z74_mulchunk(z74_bn *x, uint64_t *ch, uint32_t p) {
    if (*ch * p > 0xFFFFFFFFULL) { z74_mulu(x, (uint32_t)*ch); *ch = 1; }
    *ch *= p;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    uint64_t P = 2 * (uint64_t)n + 3;
    char *comp = (char *)calloc((size_t)P + 1, 1);
    int *pidx = (int *)malloc(((size_t)P + 1) * sizeof(int));
    uint32_t *pr = (uint32_t *)malloc(((size_t)P + 1) * sizeof(uint32_t));
    int np = 0;
    for (uint64_t i = 2; i <= P; i++) {
        if (!comp[i]) {
            pidx[i] = np;
            pr[np++] = (uint32_t)i;
            for (uint64_t q = i * i; q <= P; q += i) comp[q] = 1;
        }
    }
    pr[np] = 0xFFFFFFFFu; /* sentinel for factoring loop */

    int *ex = (int *)calloc((size_t)(n + 1) * np, sizeof(int));
    int *sg = (int *)malloc((size_t)(n + 1) * sizeof(int));
    z74_bn *num = (z74_bn *)calloc((size_t)(n + 1), sizeof(z74_bn));
    int *A = (int *)malloc((size_t)np * sizeof(int));
    int *T = (int *)malloc((size_t)np * sizeof(int));
    z74_bn zero_bn = {NULL, 0, 0};
    z74_bn acc = zero_bn, term = zero_bn, tmp = zero_bn;

    z74_reserve(&num[0], 1);
    num[0].d[0] = 1; num[0].len = 1;
    sg[0] = 1;

    for (idx_t k = 1; k <= n; k++) {
        int sa = 1;
        acc.len = 0;
        memset(A, 0, (size_t)np * sizeof(int));
        for (idx_t m = 0; m < k; m++) {
            if (num[m].len == 0) continue;
            idx_t j = k - m;
            memcpy(T, ex + (size_t)m * np, (size_t)np * sizeof(int));
            z74_addfac(T, 2 * (uint64_t)j + 3, pr, pidx);
            uint64_t ch = 1;
            for (int i = 0; i < np; i++) {
                if (T[i] > A[i]) {
                    if (acc.len)
                        for (int t = A[i]; t < T[i]; t++) z74_mulchunk(&acc, &ch, pr[i]);
                    A[i] = T[i];
                }
            }
            if (ch > 1) z74_mulu(&acc, (uint32_t)ch);
            z74_copy(&term, &num[m]);
            z74_mulu(&term, (uint32_t)(2 * (uint64_t)(n - 1) * j + 3 * (uint64_t)k));
            ch = 1;
            for (int i = 0; i < np; i++)
                for (int t = T[i]; t < A[i]; t++) z74_mulchunk(&term, &ch, pr[i]);
            if (ch > 1) z74_mulu(&term, (uint32_t)ch);
            z74_sadd(&acc, &sa, &term, -sg[m], &tmp);
        }
        z74_addfac(A, (uint64_t)k, pr, pidx);
        for (int i = 0; i < np; i++)
            while (A[i] > 0 && acc.len && z74_modu(&acc, pr[i]) == 0) {
                z74_divu(&acc, pr[i]);
                A[i]--;
            }
        if (acc.len == 0) { memset(A, 0, (size_t)np * sizeof(int)); sa = 1; }
        num[k] = acc;
        acc = zero_bn;
        sg[k] = sa;
        memcpy(ex + (size_t)k * np, A, (size_t)np * sizeof(int));
    }

    seq_t r = 1;
    for (int i = 0; i < np; i++)
        for (int t = 0; t < ex[(size_t)n * np + i]; t++) r *= (seq_t)pr[i];

    for (idx_t k = 0; k <= n; k++) free(num[k].d);
    free(num); free(acc.d); free(term.d); free(tmp.d);
    free(ex); free(sg); free(A); free(T);
    free(comp); free(pidx); free(pr);
    return r;
}
