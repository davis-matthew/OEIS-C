/*
 * A007038  Theta series of 20-dimensional lattice R_20 with det 1024 and
 *          minimal norm 4.
 * https://oeis.org/A007038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] (A^5 - 120*A^3*B + 7680*A*B^2), where A = 1 +
 *            24*Sum_{k>=1} (Sum_{d|k, d odd} d) x^k (theta series of D4,
 *            weight 2 on Gamma_0(2)), B = Sum_{k>=1} (Sum_{d|k, k/d odd} d^3)
 *            x^k (= eta(2z)^16/eta(z)^8, weight 4). The coefficients are
 *            computed exactly modulo three ~60-bit primes and reco...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 108, u128 n <= 8032,
 *            big any n
 * Helpers:   alloc_zero, is_prime, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"

typedef unsigned __int128 u128_t;

static uint64_t P_[3];
static int primes_ready = 0;
static seq_t *memo = 0;
static idx_t memo_len = 0;

static uint64_t mulmod(uint64_t a, uint64_t b, uint64_t p) {
    return (uint64_t)(((u128_t)a * b) % p);
}

static uint64_t powmod(uint64_t b, uint64_t e, uint64_t p) {
    uint64_t r = 1 % p;
    b %= p;
    while (e) {
        if (e & 1) r = mulmod(r, b, p);
        b = mulmod(b, b, p);
        e >>= 1;
    }
    return r;
}

static void init_primes(void) {
    uint64_t c = (1ULL << 60) - 1;
    int i = 0;
    while (i < 3) {
        if (is_prime(c)) P_[i++] = c;
        c -= 2;
    }
    primes_ready = 1;
}

static void conv(const uint64_t *x, const uint64_t *y, uint64_t *z, size_t len, uint64_t p) {
    size_t n, k;
    for (n = 0; n < len; n++) {
        u128_t acc = 0;
        for (k = 0; k <= n; k++) {
            acc += (u128_t)x[k] * y[n - k];
            if ((k & 127) == 127) acc %= p;
        }
        z[n] = (uint64_t)(acc % p);
    }
}

static seq_t crt3(uint64_t r1, uint64_t r2, uint64_t r3) {
    uint64_t p1 = P_[0], p2 = P_[1], p3 = P_[2];
    uint64_t inv1_2 = powmod(p1 % p2, p2 - 2, p2);
    uint64_t inv12_3 = powmod(mulmod(p1 % p3, p2 % p3, p3), p3 - 2, p3);
    uint64_t t2 = mulmod((r2 + p2 - r1 % p2) % p2, inv1_2, p2);
    uint64_t tmp = (r1 % p3 + mulmod(p1 % p3, t2, p3)) % p3;
    uint64_t t3 = mulmod((r3 + p3 - tmp) % p3, inv12_3, p3);
    if (seq_is_fixed_width()) {
        u128_t x = (u128_t)r1 + (u128_t)p1 * t2 + (u128_t)p1 * p2 * t3;
        return (seq_t)x;
    } else {
        seq_t x = (seq_t)t3;
        x *= (seq_t)p2;
        x += (seq_t)t2;
        x *= (seq_t)p1;
        x += (seq_t)r1;
        return x;
    }
}

static void compute(idx_t len) {
    size_t L = (size_t)len, k, e, m;
    int i;
    uint64_t *Ae = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t *Be = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t *a = (uint64_t *)malloc(L * sizeof(uint64_t));
    uint64_t *b = (uint64_t *)malloc(L * sizeof(uint64_t));
    uint64_t *U = (uint64_t *)malloc(L * sizeof(uint64_t));
    uint64_t *V = (uint64_t *)malloc(L * sizeof(uint64_t));
    uint64_t *W1 = (uint64_t *)malloc(L * sizeof(uint64_t));
    uint64_t *W2 = (uint64_t *)malloc(L * sizeof(uint64_t));
    uint64_t *res[3];

    for (e = 1; e < L; e += 2)
        for (m = 1; e * m < L; m++) {
            Ae[e * m] += e;
            Be[e * m] += (uint64_t)m * m * m;
        }

    for (i = 0; i < 3; i++) {
        uint64_t p = P_[i];
        res[i] = (uint64_t *)malloc(L * sizeof(uint64_t));
        for (k = 0; k < L; k++) {
            a[k] = (k == 0) ? 1 : mulmod(24, Ae[k] % p, p);
            b[k] = Be[k] % p;
        }
        conv(a, a, U, L, p);
        for (k = 0; k < L; k++)
            V[k] = (U[k] + p - mulmod(120, b[k], p)) % p;
        conv(U, V, W1, L, p);
        conv(b, b, W2, L, p);
        for (k = 0; k < L; k++)
            W1[k] = (W1[k] + mulmod(7680, W2[k], p)) % p;
        conv(a, W1, res[i], L, p);
    }

    if (memo) release(memo, memo_len);
    memo = alloc_zero(len);
    for (k = 0; k < L; k++)
        memo[k] = crt3(res[0][k], res[1][k], res[2][k]);
    memo_len = len;

    for (i = 0; i < 3; i++) free(res[i]);
    free(Ae); free(Be); free(a); free(b);
    free(U); free(V); free(W1); free(W2);
}

seq_t a(idx_t n) {
    if (!primes_ready) init_primes();
    if (n >= memo_len) {
        idx_t want = n + n / 4 + 32;
        compute(want);
    }
    return memo[n];
}
