/*
 * A002388  Decimal expansion of Pi^2.
 * https://oeis.org/A002388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   Pi^2 = 10 * Sum_{k>=0} binomial(2*k,k)*(-1/16)^k/(2*k+1)^2; a(n)
 *            = n-th decimal digit of Pi^2, computed with base-10^9
 *            fixed-point arithmetic
 * Range:     u8 n <= 9337, u16 n <= 9337, u32 n <= 9337, u64 n <= 9337,
 *            u128 n <= 9337, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A2388_B 1000000000ULL

static unsigned char *a2388_digs = NULL;
static idx_t a2388_have = 0;

static void a2388_add(uint64_t *acc, const uint64_t *x, size_t s, size_t L) {
    uint64_t c = 0;
    size_t i = L + 1;
    while (i-- > 0) {
        if (i < s && c == 0) break;
        uint64_t v = acc[i] + (i >= s ? x[i] : 0) + c;
        if (v >= A2388_B) { acc[i] = v - A2388_B; c = 1; } else { acc[i] = v; c = 0; }
    }
}

static void a2388_compute(idx_t D) {
    size_t L = (size_t)(D / 9) + 4;
    uint64_t *t = (uint64_t *)calloc(L + 1, sizeof(uint64_t));
    uint64_t *u = (uint64_t *)calloc(L + 1, sizeof(uint64_t));
    uint64_t *P = (uint64_t *)calloc(L + 1, sizeof(uint64_t));
    uint64_t *M = (uint64_t *)calloc(L + 1, sizeof(uint64_t));
    size_t s = 0, i;
    uint64_t k;
    t[0] = 1;
    P[0] = 1;
    for (k = 1;; k++) {
        uint64_t rem = 0, d = 8 * k, c = 0, q = (2 * k + 1) * (2 * k + 1);
        for (i = s; i <= L; i++) {
            uint64_t v = rem * A2388_B + t[i];
            t[i] = v / d;
            rem = v % d;
        }
        while (s <= L && t[s] == 0) s++;
        if (s > L) break;
        i = L + 1;
        while (i-- > s) {
            uint64_t v = t[i] * (2 * k - 1) + c;
            t[i] = v % A2388_B;
            c = v / A2388_B;
        }
        while (c) {
            s--;
            uint64_t v = t[s] + c;
            t[s] = v % A2388_B;
            c = v / A2388_B;
        }
        rem = 0;
        for (i = 0; i < s; i++) u[i] = 0;
        for (i = s; i <= L; i++) {
            uint64_t v = rem * A2388_B + t[i];
            u[i] = v / q;
            rem = v % q;
        }
        a2388_add((k & 1) ? M : P, u, s, L);
    }
    /* R = P - M */
    {
        uint64_t b = 0;
        i = L + 1;
        while (i-- > 0) {
            uint64_t sub = M[i] + b;
            if (P[i] >= sub) { P[i] -= sub; b = 0; }
            else { P[i] = P[i] + A2388_B - sub; b = 1; }
        }
    }
    /* times 10 */
    {
        uint64_t c = 0;
        i = L + 1;
        while (i-- > 0) {
            uint64_t v = P[i] * 10 + c;
            P[i] = v % A2388_B;
            c = v / A2388_B;
        }
        P[0] += c * A2388_B;
    }
    free(a2388_digs);
    a2388_digs = (unsigned char *)malloc((size_t)D + 1);
    a2388_digs[0] = (unsigned char)(P[0] % 10);
    {
        idx_t j;
        for (j = 1; j <= D; j++) {
            size_t limb = (size_t)((j - 1) / 9) + 1;
            int pos = (int)((j - 1) % 9);
            uint64_t v = P[limb];
            int e;
            for (e = 0; e < 8 - pos; e++) v /= 10;
            a2388_digs[j] = (unsigned char)(v % 10);
        }
    }
    a2388_have = D;
    free(t); free(u); free(P); free(M);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n + 1 > a2388_have) {
        idx_t D = (n + 40) * 2;
        if (D < 200) D = 200;
        a2388_compute(D);
    }
    return (seq_t)a2388_digs[n - 1];
}
