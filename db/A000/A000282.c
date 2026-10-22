/*
 * A000282  Finite automata.
 * https://oeis.org/A000282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = inverse Euler transform of b = A054747, where by Burnside
 *            over (state permutations s) x (input permutations of {0,1}):
 *            b(n) = (1/(2 n!)) Sum_{s in S_n} [ Prod_{cycles i of s} (2
 *            f(i))^2 + Prod_{cycles i of s} ( i odd ? 2 f(2i) : (2 f(i))^2 )
 *            ], f(L) = Sum_{d|L} d*c_d (c_d = number of d-cyc...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   bitlen64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bitlen64.h"

static int BL;
static uint32_t *S_, *T1, *T2, *T3, *CNT, *FACT;
static idx_t *pp, *pc;
static int K;
static uint32_t **cacheA = 0;
static idx_t cacheN = 0;
static int cacheL = 0;

static void bzero_(uint32_t *a) { memset(a, 0, sizeof(uint32_t) * BL); }
static void bset1(uint32_t *a) { bzero_(a); a[0] = 1; }
static void bcopy_(uint32_t *a, const uint32_t *b) { memcpy(a, b, sizeof(uint32_t) * BL); }
static int blen(const uint32_t *a) {
    int i = BL;
    while (i > 0 && a[i - 1] == 0) i--;
    return i;
}
static void bmuls(uint32_t *a, uint32_t m) {
    int l = blen(a), i;
    uint64_t c = 0;
    for (i = 0; i < l; i++) {
        uint64_t t = (uint64_t)a[i] * m + c;
        a[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (l < BL) a[l] = (uint32_t)c;
}
static void bdivs(uint32_t *a, uint32_t d) {
    int i = blen(a);
    uint64_t r = 0;
    while (i > 0) {
        i--;
        uint64_t cur = (r << 32) | a[i];
        a[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}
static void badd(uint32_t *a, const uint32_t *b) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < BL; i++) {
        uint64_t t = (uint64_t)a[i] + b[i] + c;
        a[i] = (uint32_t)t;
        c = t >> 32;
    }
}
static void badds(uint32_t *a, uint32_t s) {
    uint64_t c = s;
    int i;
    for (i = 0; i < BL && c; i++) {
        uint64_t t = (uint64_t)a[i] + c;
        a[i] = (uint32_t)t;
        c = t >> 32;
    }
}
static void bsub(uint32_t *a, const uint32_t *b) {
    int64_t br = 0;
    int i;
    for (i = 0; i < BL; i++) {
        int64_t t = (int64_t)a[i] - b[i] - br;
        if (t < 0) { t += ((int64_t)1 << 32); br = 1; } else br = 0;
        a[i] = (uint32_t)t;
    }
}
static void bmul(uint32_t *r, const uint32_t *a, const uint32_t *b) {
    int la = blen(a), lb = blen(b), i, j;
    bzero_(r);
    for (i = 0; i < la; i++) {
        uint64_t c = 0;
        for (j = 0; j < lb && i + j < BL; j++) {
            uint64_t t = (uint64_t)a[i] * b[j] + r[i + j] + c;
            r[i + j] = (uint32_t)t;
            c = t >> 32;
        }
        if (i + lb < BL) r[i + lb] = (uint32_t)c;
    }
}

/* packed multiply/divide by many small factors */
static uint64_t pk;
static void pk_op(uint32_t *a, uint64_t v, int div) {
    if (pk * v > 0xFFFFFFFFULL) {
        if (div) bdivs(a, (uint32_t)pk); else bmuls(a, (uint32_t)pk);
        pk = v;
    } else pk *= v;
}
static void pk_flush(uint32_t *a, int div) {
    if (pk > 1) { if (div) bdivs(a, (uint32_t)pk); else bmuls(a, (uint32_t)pk); }
    pk = 1;
}

static uint64_t fval(idx_t L) {
    uint64_t s = 0;
    int t;
    for (t = 0; t < K; t++)
        if (L % pp[t] == 0) s += (uint64_t)pp[t] * pc[t];
    return s;
}

static void leaf(void) {
    int t;
    idx_t e;
    bset1(T1); pk = 1;
    for (t = 0; t < K; t++) {
        uint64_t v = 2 * fval(pp[t]);
        for (e = 0; e < 2 * pc[t]; e++) pk_op(T1, v, 0);
    }
    pk_flush(T1, 0);
    bset1(T2); pk = 1;
    for (t = 0; t < K; t++) {
        if (pp[t] & 1) {
            uint64_t v = 2 * fval(2 * pp[t]);
            for (e = 0; e < pc[t]; e++) pk_op(T2, v, 0);
        } else {
            uint64_t v = 2 * fval(pp[t]);
            for (e = 0; e < 2 * pc[t]; e++) pk_op(T2, v, 0);
        }
    }
    pk_flush(T2, 0);
    badd(T1, T2);
    bcopy_(CNT, FACT); pk = 1;
    for (t = 0; t < K; t++) {
        for (e = 0; e < pc[t]; e++) pk_op(CNT, pp[t], 1);
        for (e = 2; e <= pc[t]; e++) pk_op(CNT, e, 1);
    }
    pk_flush(CNT, 1);
    bmul(T3, CNT, T1);
    badd(S_, T3);
}

static void rec(idx_t rem, idx_t maxp) {
    idx_t i, c;
    if (rem == 0) { leaf(); return; }
    for (i = (rem < maxp ? rem : maxp); i >= 1; i--)
        for (c = 1; c * i <= rem; c++) {
            pp[K] = i; pc[K] = c; K++;
            rec(rem - c * i, i - 1);
            K--;
        }
}

static void compute(idx_t N) {
    idx_t m, j, n;
    uint64_t W = 3;
    for (j = 2; j <= N; j++) W += bitlen64(j);
    W += 2 * N * (uint64_t)bitlen64(2 * N);
    BL = (int)(2 * (W / 32 + 1) + 4);

    S_ = (uint32_t *)malloc(sizeof(uint32_t) * BL);
    T1 = (uint32_t *)malloc(sizeof(uint32_t) * BL);
    T2 = (uint32_t *)malloc(sizeof(uint32_t) * BL);
    T3 = (uint32_t *)malloc(sizeof(uint32_t) * BL);
    CNT = (uint32_t *)malloc(sizeof(uint32_t) * BL);
    FACT = (uint32_t *)malloc(sizeof(uint32_t) * BL);
    pp = (idx_t *)malloc(sizeof(idx_t) * (N + 2));
    pc = (idx_t *)malloc(sizeof(idx_t) * (N + 2));

    uint32_t **B = (uint32_t **)malloc(sizeof(uint32_t *) * (N + 1));
    uint32_t **P = (uint32_t **)malloc(sizeof(uint32_t *) * (N + 1));
    uint32_t **C = (uint32_t **)malloc(sizeof(uint32_t *) * (N + 1));
    uint32_t **A = (uint32_t **)malloc(sizeof(uint32_t *) * (N + 1));
    for (m = 0; m <= N; m++) {
        B[m] = (uint32_t *)calloc(BL, sizeof(uint32_t));
        P[m] = (uint32_t *)calloc(BL, sizeof(uint32_t));
        C[m] = (uint32_t *)calloc(BL, sizeof(uint32_t));
        A[m] = (uint32_t *)calloc(BL, sizeof(uint32_t));
    }

    /* b(m) by Burnside */
    bset1(FACT);
    for (m = 1; m <= N; m++) {
        bmuls(FACT, (uint32_t)m);
        bzero_(S_);
        K = 0;
        rec(m, m);
        pk = 1;
        pk_op(S_, 2, 1);
        for (j = m; j >= 2; j--) pk_op(S_, j, 1);
        pk_flush(S_, 1);
        bcopy_(B[m], S_);
    }

    /* inverse Euler transform */
    bset1(P[0]);
    for (n = 1; n <= N; n++) {
        bcopy_(A[n], B[n]);
        bsub(A[n], P[n]);
        idx_t J = N / n;
        bset1(C[0]);
        for (j = 1; j <= J; j++) {
            bcopy_(T1, A[n]);
            badds(T1, (uint32_t)(j - 1));
            bmul(C[j], C[j - 1], T1);
            bdivs(C[j], (uint32_t)j);
        }
        for (m = N; m >= n; m--) {
            for (j = 1; j * n <= m; j++) {
                bmul(T2, C[j], P[m - j * n]);
                badd(P[m], T2);
            }
        }
    }

    if (cacheA) {
        for (m = 0; m <= cacheN; m++) free(cacheA[m]);
        free(cacheA);
    }
    cacheA = A;
    cacheN = N;
    cacheL = BL;

    for (m = 0; m <= N; m++) { free(B[m]); free(P[m]); free(C[m]); }
    free(B); free(P); free(C);
    free(S_); free(T1); free(T2); free(T3); free(CNT); free(FACT);
    free(pp); free(pc);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (!cacheA || n > cacheN) compute(n);
    const uint32_t *x = cacheA[n];
    int i = cacheL;
    while (i > 0 && x[i - 1] == 0) i--;
    seq_t r = 0;
    while (i > 0) {
        int sh;
        i--;
        for (sh = 24; sh >= 0; sh -= 8)
            r = r * 256 + (int)((x[i] >> sh) & 255u);
    }
    return r;
}
