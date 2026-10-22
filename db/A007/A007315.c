/*
 * A007315  Reversion of g.f. (beginning with constant term) for number of
 *          trees with n nodes.
 * https://oeis.org/A007315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1/n) [x^(n-1)] (1/T(x))^n (Lagrange inversion), T(x) =
 *            Sum_{k>=0} A000055(k) x^k, A000055(m) = r(m) - (Sum_{i=1..m-1}
 *            r(i) r(m-i) - [m even] r(m/2))/2, A000055(0) = 1, r = A000081:
 *            r(1) = 1, r(m+1) = (1/m) Sum_{k=1..m} (Sum_{d|k} d r(d))
 *            r(m+1-k); power coefficients via P(0) = 1, P(k) = (...
 * Range:     i8 n <= 9, i16 n <= 15, i32 n <= 28, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   alloc_zero, from_i128, mconv, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_is_big.h"
#include "../utils/from_i128.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/mconv.h"

static const uint64_t PR[5] = {2305843009213693951ULL, 2147483647ULL, 998244353ULL,
                               1000000007ULL, 1000000009ULL};

static uint64_t mmul(uint64_t x, uint64_t y, uint64_t p) {
    return (uint64_t)(((unsigned __int128)x * y) % p);
}

static uint64_t mpw(uint64_t x, uint64_t e, uint64_t p) {
    uint64_t r = 1 % p;
    x %= p;
    while (e) {
        if (e & 1) r = mmul(r, x, p);
        x = mmul(x, x, p);
        e >>= 1;
    }
    return r;
}

static uint64_t minv(uint64_t x, uint64_t p) { return mpw(x % p, p - 2, p); }

static uint64_t solve_mod(idx_t N, uint64_t p) {
    idx_t L = N + 1;
    uint64_t *r = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t *b = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t *t = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t *h = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t *hh = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t *P = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t inv2 = (p + 1) / 2;

    r[1] = 1;
    for (idx_t q = 1; q <= N; q++) b[q] = (b[q] + 1) % p;
    for (idx_t m = 1; m < N; m++) {
        uint64_t s = mconv(b, r, 1, m, m + 1, p);
        idx_t d = m + 1;
        r[d] = mmul(s, minv(m % p, p), p);
        uint64_t w = mmul(d % p, r[d], p);
        for (idx_t q = d; q <= N; q += d) b[q] = (b[q] + w) % p;
    }

    t[0] = 1;
    for (idx_t m = 1; m < N; m++) {
        uint64_t s = mconv(r, r, 1, m - 1, m, p);
        if (m % 2 == 0) s = (s + p - r[m / 2]) % p;
        t[m] = (r[m] + p - mmul(s, inv2, p)) % p;
    }

    h[0] = 1;
    for (idx_t m = 1; m < N; m++) {
        uint64_t s = mconv(t, h, 1, m, m, p);
        h[m] = (p - s) % p;
    }

    for (idx_t j = 0; j < N; j++) {
        uint64_t c = (uint64_t)(((unsigned __int128)(N + 1) * j) % p);
        hh[j] = mmul(c, h[j], p);
    }

    P[0] = 1;
    for (idx_t k = 1; k < N; k++) {
        uint64_t s1 = mconv(hh, P, 1, k, k, p);
        uint64_t s2 = mconv(h, P, 1, k, k, p);
        uint64_t s = (s1 + p - mmul(k % p, s2, p)) % p;
        P[k] = mmul(s, minv(k % p, p), p);
    }

    uint64_t res = mmul(P[N - 1], minv(N % p, p), p);
    free(r); free(b); free(t); free(h); free(hh); free(P);
    return res;
}

static seq_t solve_big(idx_t N) {
    idx_t L = N + 1;
    seq_t *r = alloc_zero(L);
    seq_t *b = alloc_zero(L);
    seq_t *t = alloc_zero(L);
    seq_t *h = alloc_zero(L);
    seq_t *P = alloc_zero(L);

    r[1] = 1;
    for (idx_t q = 1; q <= N; q++) b[q] += 1;
    for (idx_t m = 1; m < N; m++) {
        seq_t s = 0;
        for (idx_t k = 1; k <= m; k++) s += b[k] * r[m + 1 - k];
        idx_t d = m + 1;
        r[d] = s / (seq_t)m;
        seq_t w = (seq_t)d * r[d];
        for (idx_t q = d; q <= N; q += d) b[q] += w;
    }

    t[0] = 1;
    for (idx_t m = 1; m < N; m++) {
        seq_t s = 0;
        for (idx_t i = 1; i < m; i++) s += r[i] * r[m - i];
        if (m % 2 == 0) s -= r[m / 2];
        t[m] = r[m] - s / 2;
    }

    h[0] = 1;
    for (idx_t m = 1; m < N; m++) {
        seq_t s = 0;
        for (idx_t j = 1; j <= m; j++) s += t[j] * h[m - j];
        h[m] = -s;
    }

    P[0] = 1;
    for (idx_t k = 1; k < N; k++) {
        seq_t s = 0;
        for (idx_t j = 1; j <= k; j++)
            s += ((seq_t)(N + 1) * (seq_t)j - (seq_t)k) * h[j] * P[k - j];
        P[k] = s / (seq_t)k;
    }

    seq_t res = P[N - 1] / (seq_t)N;
    release(r, L); release(b, L); release(t, L); release(h, L); release(P, L);
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (seq_is_big()) return solve_big(n);

    int bits = (int)(8 * sizeof(seq_t));
    int k = bits <= 32 ? 1 : (bits <= 64 ? 2 : 4);
    uint64_t v[5];
    for (int i = 0; i < k; i++) {
        uint64_t pi = PR[i];
        uint64_t x = solve_mod(n, pi);
        for (int j = 0; j < i; j++) {
            x = (x + pi - v[j] % pi) % pi;
            x = mmul(x, minv(PR[j] % pi, pi), pi);
        }
        v[i] = x;
    }
    int neg = v[k - 1] > PR[k - 1] / 2;
    unsigned __int128 acc = v[k - 1];
    for (int i = k - 2; i >= 0; i--) acc = acc * PR[i] + v[i];
    if (neg) {
        unsigned __int128 M = 1;
        for (int i = 0; i < k; i++) M *= PR[i];
        acc -= M;
    }
    return from_i128((__int128)acc);
}
