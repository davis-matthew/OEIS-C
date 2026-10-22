/*
 * A007563  Number of rooted connected graphs where every block is a complete
 *          graph.
 * https://oeis.org/A007563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(n+1) = EULER(EULER(a))(n), with a(0) = 0, a(1) = 1. Let b =
 *            EULER(a), so B(x) = Prod_{k>=1} (1-x^k)^(-a(k)). Then Sum_{n>=0}
 *            a(n+1) x^n = Prod_{k>=1} (1-x^k)^(-b(k)). Big-integer build:
 *            e(n) = (1/n) Sum_{j=1..n} c(j) e(n-j), where c(j) = Sum_{d|j}
 *            d*s(d). Fixed-width build: the products are mul...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"

static uint64_t small_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

/* P(x) *= (1-x^k)^(-m), truncated to len coefficients */
static void mulin(seq_t *P, idx_t len, idx_t k, seq_t m, seq_t *coef)
{
    idx_t T, t, j;
    if (m == 0) return;
    T = (len - 1) / k;
    coef[0] = 1;
    for (t = 0; t < T; t++) {
        uint64_t d = (uint64_t)t + 1;
        uint64_t r = (uint64_t)(long long)(coef[t] % d);
        uint64_t g = small_gcd(r, d);
        uint64_t dd = d / g;
        seq_t q = (m + t) / dd;
        coef[t + 1] = (coef[t] / g) * q;
    }
    for (j = len; j-- > 0;) {
        seq_t s = P[j];
        for (t = 1; t * k <= j; t++)
            s += coef[t] * P[j - t * k];
        P[j] = s;
    }
}

static seq_t fixed_a(idx_t n)
{
    seq_t *A, *B, *PA, *PB, *coef, res;
    idx_t K, len = n + 1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n > 140) return 0; /* far beyond any fixed width */
    A = alloc_zero(len); B = alloc_zero(len);
    PA = alloc_zero(len); PB = alloc_zero(len); coef = alloc_zero(len);
    A[1] = 1; PA[0] = 1; PB[0] = 1;
    for (K = 1; K + 1 <= n; K++) {
        mulin(PA, n, K, A[K], coef);
        B[K] = PA[K];
        mulin(PB, n, K, B[K], coef);
        A[K + 1] = PB[K];
    }
    res = A[n];
    release(A, len); release(B, len); release(PA, len);
    release(PB, len); release(coef, len);
    return res;
}

static seq_t *BA = 0, *BB = 0, *CA = 0, *CB = 0;
static idx_t BL = 0, BCAP = 0;

static seq_t big_a(idx_t n)
{
    if (BL == 0) {
        BCAP = 16;
        BA = alloc_zero(BCAP); BB = alloc_zero(BCAP);
        CA = alloc_zero(BCAP); CB = alloc_zero(BCAP);
        BA[0] = 0; BA[1] = 1; BB[0] = 1;
        BL = 1;
    }
    while (BL < n) {
        idx_t K = BL, d, j;
        seq_t s;
        if (K + 2 > BCAP) {
            idx_t nc = BCAP * 2;
            while (K + 2 > nc) nc *= 2;
            BA = grow_zero(BA, BCAP, nc); BB = grow_zero(BB, BCAP, nc);
            CA = grow_zero(CA, BCAP, nc); CB = grow_zero(CB, BCAP, nc);
            BCAP = nc;
        }
        s = 0;
        for (d = 1; d * d <= K; d++)
            if (K % d == 0) {
                idx_t e = K / d;
                s += BA[d] * d;
                if (e != d) s += BA[e] * e;
            }
        CA[K] = s;
        s = 0;
        for (j = 1; j <= K; j++) s += CA[j] * BB[K - j];
        BB[K] = s / K;
        s = 0;
        for (d = 1; d * d <= K; d++)
            if (K % d == 0) {
                idx_t e = K / d;
                s += BB[d] * d;
                if (e != d) s += BB[e] * e;
            }
        CB[K] = s;
        s = 0;
        for (j = 1; j <= K; j++) s += CB[j] * BA[K - j + 1];
        BA[K + 1] = s / K;
        BL++;
    }
    return BA[n];
}

seq_t a(idx_t n)
{
    if (seq_is_fixed_width()) return fixed_a(n);
    return big_a(n);
}
