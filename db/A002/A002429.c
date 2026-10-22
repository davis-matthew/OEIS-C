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
 * Helpers:   q2429_add, q2429_div_small, q2429_mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/q2429_add.h"
#include "../utils/q2429_div_small.h"
#include "../utils/q2429_mul_small.h"

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
