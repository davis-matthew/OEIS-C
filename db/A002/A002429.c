/*
 * A002429  Numerators of double sums of reciprocals.
 * https://oeis.org/A002429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numerator of 3/(2n+3) * Sum_{m=1..n+1} (1/m) *
 *            Sum_{k=1..m} 1/(2k-1), i.e. the absolute value of the
 *            coefficient of x^(2n+3) in arctan(x)^3 (exact multi-limb
 *            rationals)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 545 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static size_t q2429_add(uint32_t *a, size_t la, const uint32_t *b, size_t lb) {
    size_t n = la > lb ? la : lb, i;
    uint64_t c = 0;
    for (i = 0; i < n; i++) {
        c += (uint64_t)(i < la ? a[i] : 0) + (i < lb ? b[i] : 0);
        a[i] = (uint32_t)c;
        c >>= 32;
    }
    if (c) a[n++] = (uint32_t)c;
    return n;
}

/* q = x / d (q may alias x or be NULL), returns remainder */
static uint32_t q2429_div_small(uint32_t *q, const uint32_t *x, size_t len, uint32_t d, size_t *qlen) {
    uint64_t r = 0;
    size_t i = len;
    while (i > 0) {
        i--;
        r = (r << 32) | x[i];
        if (q) q[i] = (uint32_t)(r / d);
        r %= d;
    }
    if (q && qlen) {
        size_t l = len;
        while (l > 0 && q[l - 1] == 0) l--;
        *qlen = l;
    }
    return (uint32_t)r;
}

static size_t q2429_mul_small(uint32_t *x, size_t len, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < len; i++) {
        c += (uint64_t)x[i] * m;
        x[i] = (uint32_t)c;
        c >>= 32;
    }
    while (c) { x[len++] = (uint32_t)c; c >>= 32; }
    return len;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n + 1, M = 2 * N + 1, top = 2 * N;
    uint64_t i, j, m;
    char *comp = (char *)calloc((size_t)M + 1, 1);
    size_t capL = (size_t)(top / 8) + 16;
    uint32_t *L = (uint32_t *)calloc(capL, sizeof(uint32_t));
    size_t lL = 1;
    L[0] = 1;
    for (i = 2; i <= M; i++) {
        if (comp[i]) continue;
        for (j = i * i; j <= M; j += i) comp[j] = 1;
        if (i <= top) {
            uint64_t pk = i;
            while (pk * i <= top) pk *= i;
            while (pk > 1) { lL = q2429_mul_small(L, lL, (uint32_t)i); pk /= i; }
        }
    }

    size_t cap = lL + 6;
    uint32_t *X = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *Q = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *Rs = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *t1 = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *t2 = (uint32_t *)calloc(cap, sizeof(uint32_t));
    size_t lX = 0, lQ = 0, lR = 0, l1, l2;

    for (m = 1; m <= N; m++) {
        uint32_t r;
        q2429_div_small(t1, L, lL, (uint32_t)(2 * m - 1), &l1);
        lX = q2429_add(X, lX, t1, l1);
        r = q2429_div_small(t2, X, lX, (uint32_t)m, &l2);
        lQ = q2429_add(Q, lQ, t2, l2);
        if (r) {
            q2429_div_small(t1, L, lL, (uint32_t)m, &l1);
            l1 = q2429_mul_small(t1, l1, r);
            lR = q2429_add(Rs, lR, t1, l1);
        }
    }

    /* Num = 3 * (Q*L + Rs) */
    size_t capN = lQ + lL + 8, lN;
    uint32_t *Num = (uint32_t *)calloc(capN, sizeof(uint32_t));
    for (i = 0; i < lQ; i++) {
        uint64_t c = 0;
        size_t k;
        for (k = 0; k < lL; k++) {
            c += (uint64_t)Q[i] * L[k] + Num[i + k];
            Num[i + k] = (uint32_t)c;
            c >>= 32;
        }
        k = i + lL;
        while (c) {
            c += Num[k];
            Num[k] = (uint32_t)c;
            c >>= 32;
            k++;
        }
    }
    lN = lQ + lL + 1;
    while (lN > 0 && Num[lN - 1] == 0) lN--;
    lN = q2429_add(Num, lN, Rs, lR);
    lN = q2429_mul_small(Num, lN, 3);

    /* denominator = (2n+3) * L^2 ; cancel common primes */
    for (i = 2; i <= M; i++) {
        if (comp[i]) continue;
        int e = 0;
        if (i <= top) {
            uint64_t pk = i;
            e = 1;
            while (pk * i <= top) { pk *= i; e++; }
            e *= 2;
        }
        {
            uint64_t t = M;
            while (t % i == 0) { e++; t /= i; }
        }
        while (e > 0 && lN > 0 && q2429_div_small(NULL, Num, lN, (uint32_t)i, NULL) == 0) {
            q2429_div_small(Num, Num, lN, (uint32_t)i, &lN);
            e--;
        }
    }

    seq_t res = 0;
    size_t k = lN;
    while (k > 0) {
        int s;
        k--;
        for (s = 24; s >= 0; s -= 8) {
            idx_t b = (idx_t)((Num[k] >> s) & 0xFFu);
            res = res * 256 + b;
        }
    }

    free(comp); free(L); free(X); free(Q); free(Rs); free(t1); free(t2); free(Num);
    return res;
}
