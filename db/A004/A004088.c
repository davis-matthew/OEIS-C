/*
 * A004088  Sum of digits of number of partitions of n.
 * https://oeis.org/A004088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007953(A000041(n)), where p(n) = A000041(n) is computed
 *            exactly in base 10^9 by Euler's pentagonal recurrence p(m) =
 *            Sum_{k>=1} (-1)^(k+1) (p(m-k(3k-1)/2) + p(m-k(3k+1)/2)), p(0) =
 *            1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint32_t *P = NULL;   /* P[m*L .. m*L+L-1]: limbs of p(m), base 1e9, little-endian */
static idx_t cap = 0;        /* p(0..cap-1) computed */
static size_t L = 0;

static void ensure(idx_t n) {
    idx_t N, m, k, s;
    size_t i;
    uint64_t *pos, *neg;
    if (n < cap) return;
    N = n + 1;
    if (N < 2 * cap) N = 2 * cap;
    if (N < 64) N = 64;
    s = 1;
    while (s * s <= N) s++;
    L = (size_t)((115 * s / 100 + 10) / 9 + 2);
    free(P);
    P = (uint32_t *)calloc((size_t)N * L, sizeof(uint32_t));
    pos = (uint64_t *)malloc(L * sizeof(uint64_t));
    neg = (uint64_t *)malloc(L * sizeof(uint64_t));
    P[0] = 1;
    for (m = 1; m < N; m++) {
        uint64_t carry;
        int64_t borrow;
        memset(pos, 0, L * sizeof(uint64_t));
        memset(neg, 0, L * sizeof(uint64_t));
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2, g2 = g1 + k;
            uint64_t *t = (k & 1) ? pos : neg;
            const uint32_t *src;
            if (g1 > m) break;
            src = P + (size_t)(m - g1) * L;
            for (i = 0; i < L; i++) t[i] += src[i];
            if (g2 <= m) {
                src = P + (size_t)(m - g2) * L;
                for (i = 0; i < L; i++) t[i] += src[i];
            }
        }
        carry = 0;
        for (i = 0; i < L; i++) {
            pos[i] += carry; carry = pos[i] / 1000000000u; pos[i] %= 1000000000u;
        }
        carry = 0;
        for (i = 0; i < L; i++) {
            neg[i] += carry; carry = neg[i] / 1000000000u; neg[i] %= 1000000000u;
        }
        borrow = 0;
        for (i = 0; i < L; i++) {
            int64_t d = (int64_t)pos[i] - (int64_t)neg[i] - borrow;
            if (d < 0) { d += 1000000000; borrow = 1; } else borrow = 0;
            P[(size_t)m * L + i] = (uint32_t)d;
        }
    }
    free(pos);
    free(neg);
    cap = N;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    size_t i;
    ensure(n);
    for (i = 0; i < L; i++) {
        uint32_t v = P[(size_t)n * L + i];
        while (v > 0) {
            sum += (seq_t)(v % 10);
            v /= 10;
        }
    }
    return sum;
}
