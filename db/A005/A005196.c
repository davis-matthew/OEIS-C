/*
 * A005196  a(n) = Sum_t t*F(n,t), where F(n,t) (see A095133) is the number of
 *          forests with n (unlabeled) nodes and exactly t trees.
 * https://oeis.org/A005196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = [x^n] F(x) * Sum_{k>=1} u(k) x^k/(1-x^k), where u =
 *            A000055 (unrooted trees), F(x) = Prod_{k>=1} (1-x^k)^(-u(k))
 *            (forests, A005195); r = A000081 rooted trees with r(n+1) = (1/n)
 *            Sum_{k=1..n} (Sum_{d|k} d r(d)) r(n-k+1), r(1) = 1, and u(n) =
 *            r(n) - Sum_{i<n/2} r(i) r(n-i) - [n even] r(n/2)(...
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 27, u64 n <= 49, u128 n <= 91,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *A_cache = NULL;
static idx_t A_N = 0;

static uint64_t small_gcd(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

/* G(x) *= (1 - x^K)^(-v), truncated at degree N, without overflowing intermediates */
static void euler_step(seq_t *G, idx_t N, idx_t K, seq_t v) {
    idx_t J = N / K;
    seq_t *mc = alloc_zero(J + 1);
    mc[0] = 1;
    for (idx_t j = 1; j <= J; j++) {
        if ((idx_t)(seq_t)j != j) { mc[j] = 0; continue; }
        seq_t b = mc[j - 1];
        seq_t sj = (seq_t)j;
        uint64_t rem = (uint64_t)(b % sj);
        uint64_t g = small_gcd(rem, (uint64_t)j);
        seq_t bp = b / (seq_t)g;
        seq_t jp = (seq_t)(j / g);
        seq_t t = (v + (seq_t)(j - 1)) / jp;
        mc[j] = bp * t;
    }
    for (idx_t m = N; m >= K; m--) {
        seq_t acc = 0;
        for (idx_t j = 1; j * K <= m; j++) acc += mc[j] * G[m - j * K];
        G[m] += acc;
        if (m == K) break;
    }
    release(mc, J + 1);
}

static void compute(idx_t N) {
    bool fixed = seq_is_fixed_width();
    seq_t *r = alloc_zero(N + 2);
    seq_t *u = alloc_zero(N + 2);
    seq_t *F = alloc_zero(N + 2);
    seq_t *c = alloc_zero(N + 2);

    if (fixed) {
        seq_t *G = alloc_zero(N + 2);
        G[0] = 1;
        for (idx_t K = 1; K <= N; K++) {
            r[K] = G[K - 1];
            euler_step(G, N, K, r[K]);
        }
        release(G, N + 2);
    } else {
        r[1] = 1;
        for (idx_t m = 1; m < N; m++) {
            seq_t cm = 0;
            for (idx_t d = 1; d * d <= m; d++) {
                if (m % d == 0) {
                    cm += (seq_t)d * r[d];
                    idx_t e = m / d;
                    if (e != d) cm += (seq_t)e * r[e];
                }
            }
            c[m] = cm;
            seq_t s = 0;
            for (idx_t k = 1; k <= m; k++) s += c[k] * r[m - k + 1];
            r[m + 1] = s / (seq_t)m;
        }
    }

    for (idx_t n = 1; n <= N; n++) {
        seq_t v = r[n];
        for (idx_t i = 1; 2 * i < n; i++) v -= r[i] * r[n - i];
        if (n % 2 == 0) {
            seq_t h = r[n / 2];
            seq_t p;
            if (h % 2 == 0) p = (h / 2) * (h - 1);
            else p = h * ((h - 1) / 2);
            v -= p;
        }
        u[n] = v;
    }

    if (fixed) {
        F[0] = 1;
        for (idx_t K = 1; K <= N; K++) euler_step(F, N, K, u[K]);
    } else {
        for (idx_t k = 0; k <= N; k++) c[k] = 0;
        for (idx_t d = 1; d <= N; d++)
            for (idx_t k = d; k <= N; k += d) c[k] += (seq_t)d * u[d];
        F[0] = 1;
        for (idx_t m = 1; m <= N; m++) {
            seq_t s = 0;
            for (idx_t k = 1; k <= m; k++) s += c[k] * F[m - k];
            F[m] = s / (seq_t)m;
        }
    }

    /* s[j] = Sum_{d|j} u(d), reuse c */
    for (idx_t k = 0; k <= N; k++) c[k] = 0;
    for (idx_t d = 1; d <= N; d++)
        for (idx_t k = d; k <= N; k += d) c[k] += u[d];

    seq_t *A = alloc_zero(N + 1);
    for (idx_t m = 1; m <= N; m++) {
        seq_t s = 0;
        for (idx_t j = 1; j <= m; j++) s += c[j] * F[m - j];
        A[m] = s;
    }

    release(r, N + 2);
    release(u, N + 2);
    release(F, N + 2);
    release(c, N + 2);

    if (A_cache) release(A_cache, A_N + 1);
    A_cache = A;
    A_N = N;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n > A_N) compute(n);
    return A_cache[n];
}
