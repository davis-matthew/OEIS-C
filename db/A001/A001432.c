/*
 * A001432  Number of graphs with n nodes and n-4 edges.
 * https://oeis.org/A001432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Polya enumeration over the pair group. a(n) = Sum_{lambda
 *            partition of n} [x^(n-4)] Prod_{edge cycles of length L} (1 +
 *            x^L) / z(lambda), where a cycle of length c gives floor((c-1)/2)
 *            edge cycles of length c (plus one of length c/2 if c is even),
 *            and two cycles of lengths b, c give gcd(b,c) edge...
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 23, u64 n <= 36, u128 n <= 40,
 *            big any n
 * Helpers:   a1432_blen, a1432_gcd, a1432_isprime, a1432_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a1432_blen.h"
#include "../utils/a1432_gcd.h"
#include "../utils/a1432_isprime.h"
#include "../utils/a1432_pow.h"

static uint64_t a1432_p;
static int a1432_N, a1432_E;
static int *a1432_parts, *a1432_mult;
static uint64_t *a1432_poly, *a1432_Q, *a1432_T, *a1432_B, *a1432_inv, *a1432_fact;
static uint64_t a1432_S;

/* R *= (1 + x^L)^cnt, truncated to degree E */
static void a1432_mulf(uint64_t *R, int L, int cnt) {
    int i, E = a1432_E;
    uint64_t p = a1432_p;
    if (L > E) return;
    for (; cnt > 0; cnt--) {
        for (i = E; i >= L; i--) {
            uint64_t v = R[i] + R[i - L];
            if (v >= p) v -= p;
            R[i] = v;
        }
    }
}

/* Q *= (1 + x^c)^k, truncated to degree E */
static void a1432_powmul(uint64_t *Q, int c, uint64_t k) {
    int E = a1432_E, i, j, jm = E / c;
    uint64_t p = a1432_p;
    uint64_t *b = a1432_B, *T = a1432_T;
    if ((uint64_t)jm > k) jm = (int)k;
    b[0] = 1;
    for (j = 1; j <= jm; j++)
        b[j] = b[j - 1] * ((k - (uint64_t)j + 1) % p) % p * a1432_inv[j] % p;
    for (i = 0; i <= E; i++) {
        uint64_t acc = 0;
        for (j = 0; j <= jm && j * c <= i; j++) {
            acc += b[j] * Q[i - j * c];
            if ((j & 31) == 31) acc %= p;
        }
        T[i] = acc % p;
    }
    memcpy(Q, T, sizeof(uint64_t) * (size_t)(E + 1));
}

static void a1432_leaf(int depth, int sum, uint64_t z) {
    int E = a1432_E, r = a1432_N - sum, idx, j;
    uint64_t p = a1432_p, K2, bj, coef = 0, zz;
    uint64_t *Q = a1432_Q;
    memcpy(Q, a1432_poly + (size_t)depth * (size_t)(E + 1), sizeof(uint64_t) * (size_t)(E + 1));
    if (r > 0) {
        idx = 0;
        while (idx < depth) {
            int c = a1432_parts[idx], idx2 = idx;
            while (idx2 < depth && a1432_parts[idx2] == c) idx2++;
            if (c <= E) a1432_powmul(Q, c, (uint64_t)r * (uint64_t)(idx2 - idx));
            idx = idx2;
        }
    }
    K2 = (uint64_t)r * (uint64_t)(r > 0 ? r - 1 : 0) / 2;
    bj = 1;
    for (j = 0; j <= E; j++) {
        if (j > 0) {
            if ((uint64_t)j > K2) bj = 0;
            else bj = bj * ((K2 - (uint64_t)j + 1) % p) % p * a1432_inv[j] % p;
        }
        if (bj == 0 && (uint64_t)j > K2) break;
        coef = (coef + Q[E - j] * bj) % p;
    }
    zz = z * a1432_fact[r] % p;
    a1432_S = (a1432_S + coef * a1432_pow(zz, p - 2, p)) % p;
}

static void a1432_dfs(int depth, int sum, int maxc, uint64_t z) {
    int E = a1432_E, c, j, hi;
    uint64_t p = a1432_p;
    a1432_leaf(depth, sum, z);
    hi = maxc;
    if (hi > a1432_N - sum) hi = a1432_N - sum;
    for (c = hi; c >= 2; c--) {
        uint64_t *src = a1432_poly + (size_t)depth * (size_t)(E + 1);
        uint64_t *dst = src + (E + 1);
        int m;
        memcpy(dst, src, sizeof(uint64_t) * (size_t)(E + 1));
        if (c & 1) {
            a1432_mulf(dst, c, (c - 1) / 2);
        } else {
            a1432_mulf(dst, c, (c - 2) / 2);
            a1432_mulf(dst, c / 2, 1);
        }
        for (j = 0; j < depth; j++) {
            int q = a1432_parts[j];
            int g = a1432_gcd(c, q);
            long long L = (long long)(c / g) * q;
            if (L <= E) a1432_mulf(dst, (int)L, g);
        }
        a1432_parts[depth] = c;
        m = (depth > 0 && a1432_parts[depth - 1] == c) ? a1432_mult[depth - 1] + 1 : 1;
        a1432_mult[depth] = m;
        a1432_dfs(depth + 1, sum + c, c, z * (uint64_t)c % p * (uint64_t)m % p);
    }
}

seq_t a(idx_t n) {
    int N, E, K, i, j, bits, rows;
    uint64_t M, cand;
    uint64_t *pr, *res, *v;
    seq_t out;

    if (n < 4) return 0;
    N = (int)n;
    E = N - 4;
    M = (uint64_t)N * (uint64_t)(N - 1) / 2;

    bits = 2;
    for (j = 1; j <= E; j++)
        bits += a1432_blen(M - (uint64_t)j + 1) - a1432_blen((uint64_t)j) + 1;
    K = bits / 28 + 2;

    pr = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)K);
    res = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)K);
    v = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)K);
    cand = ((uint64_t)1 << 29) - 1;
    for (i = 0; i < K; i++) {
        while (!a1432_isprime(cand)) cand -= 2;
        pr[i] = cand;
        cand -= 2;
    }

    rows = N / 2 + 2;
    a1432_N = N;
    a1432_E = E;
    a1432_parts = (int *)malloc(sizeof(int) * (size_t)rows);
    a1432_mult = (int *)malloc(sizeof(int) * (size_t)rows);
    a1432_poly = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)rows * (size_t)(E + 1));
    a1432_Q = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(E + 1));
    a1432_T = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(E + 1));
    a1432_B = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(E + 2));
    a1432_inv = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(E + 2));
    a1432_fact = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(N + 1));

    for (i = 0; i < K; i++) {
        uint64_t p = pr[i];
        a1432_p = p;
        a1432_inv[0] = 1;
        for (j = 1; j <= E; j++) a1432_inv[j] = a1432_pow((uint64_t)j, p - 2, p);
        a1432_fact[0] = 1;
        for (j = 1; j <= N; j++) a1432_fact[j] = a1432_fact[j - 1] * (uint64_t)j % p;
        for (j = 0; j <= E; j++) a1432_poly[j] = 0;
        a1432_poly[0] = 1;
        a1432_S = 0;
        a1432_dfs(0, 0, N, 1);
        res[i] = a1432_S;
    }

    /* Garner mixed-radix digits */
    for (i = 0; i < K; i++) {
        uint64_t pi = pr[i], x = res[i] % pi;
        for (j = 0; j < i; j++) {
            uint64_t inv = a1432_pow(pr[j] % pi, pi - 2, pi);
            x = (x + pi - v[j] % pi) % pi * inv % pi;
        }
        v[i] = x;
    }
    out = 0;
    out += (idx_t)v[K - 1];
    for (i = K - 2; i >= 0; i--) {
        out = out * (idx_t)pr[i];
        out += (idx_t)v[i];
    }

    free(pr); free(res); free(v);
    free(a1432_parts); free(a1432_mult); free(a1432_poly);
    free(a1432_Q); free(a1432_T); free(a1432_B); free(a1432_inv); free(a1432_fact);
    return out;
}
