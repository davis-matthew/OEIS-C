/*
 * A006887  Kaprekar triples: q such that q = x + y + z and q^3 = x*10^2n +
 *          y*10^n + z, where z < 10^n and n is the number of digits in q. q
 *          is not a power of 10 (except q=1).
 * https://oeis.org/A006887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search by digit length n. Since q^3 = x*B^2 + y*B + z with B =
 *            10^n and x + y + z = q, we get q^3 == q (mod 10^n - 1). Factor
 *            10^n - 1, take every CRT combination of q == 0, 1, -1 modulo
 *            each prime power, and keep the n-digit candidates whose
 *            base-10^n digits of q^3 add up to q
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 147, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   modinv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/modinv.h"

typedef unsigned __int128 u128_t;

static uint64_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static int done_digits = 0;

static uint64_t mulmod(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)((u128_t)a * b % m);
}

static uint64_t powmod(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = mulmod(r, b, m);
        b = mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool mr_prime(uint64_t n) {
    static const uint64_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    if (n < 2) return false;
    for (int i = 0; i < 12; i++) {
        if (n % bases[i] == 0) return n == bases[i];
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 12; i++) {
        uint64_t x = powmod(bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = mulmod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static uint64_t gcd64(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t rho(uint64_t n) {
    if (n % 2 == 0) return 2;
    for (uint64_t c = 1;; c++) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            d = gcd64(x > y ? x - y : y - x, n);
        }
        if (d != n) return d;
    }
}

static void factor(uint64_t n, uint64_t *pr, int *np) {
    if (n == 1) return;
    for (uint64_t p = 2; p < 1000 && p * p <= n; p++) {
        while (n % p == 0) { pr[(*np)++] = p; n /= p; }
    }
    if (n == 1) return;
    if (mr_prime(n)) { pr[(*np)++] = n; return; }
    uint64_t d = rho(n);
    factor(d, pr, np);
    factor(n / d, pr, np);
}

static bool check(uint64_t q, uint64_t B) {
    u128_t q2 = (u128_t)q * q;
    u128_t s = q2 / B, t = q2 % B;
    u128_t tq = t * q;
    u128_t u = tq / B, z = tq % B;
    u128_t w = s * q + u;
    u128_t y = w % B, x = w / B;
    return x + y + z == (u128_t)q;
}

static int cmp_u64(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}

static uint64_t g_M, g_B, g_lo;
static int g_nd, g_k;
static uint64_t g_mod[64], g_c[64];
static uint64_t *g_found;
static int g_nfound;

static void enumerate(int i, uint64_t s) {
    if (i == g_k) {
        uint64_t q = (s == 0) ? g_M : s;
        if (q < g_lo) return;
        if (g_nd >= 2 && q == g_lo) return;
        if (check(q, g_B)) g_found[g_nfound++] = q;
        return;
    }
    uint64_t r[3];
    r[0] = 0; r[1] = 1; r[2] = g_mod[i] - 1;
    for (int j = 0; j < 3; j++) {
        uint64_t t = mulmod(r[j], g_c[i], g_M);
        enumerate(i + 1, (uint64_t)(((u128_t)s + t) % g_M));
    }
}

static void push(uint64_t v) {
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 64;
        cache = (uint64_t *)realloc(cache, cache_cap * sizeof(uint64_t));
    }
    cache[cache_len++] = v;
}

static void do_digits(int nd) {
    uint64_t B = 1;
    for (int i = 0; i < nd; i++) B *= 10;
    uint64_t M = B - 1;
    uint64_t pr[128];
    int np = 0;
    factor(M, pr, &np);
    qsort(pr, (size_t)np, sizeof(uint64_t), cmp_u64);
    int k = 0;
    for (int i = 0; i < np;) {
        uint64_t p = pr[i], pe = 1;
        while (i < np && pr[i] == p) { pe *= p; i++; }
        g_mod[k++] = pe;
    }
    for (int i = 0; i < k; i++) {
        uint64_t Mi = M / g_mod[i];
        uint64_t inv = modinv(Mi % g_mod[i], g_mod[i]);
        g_c[i] = mulmod(Mi, inv, M);
    }
    size_t total = 1;
    for (int i = 0; i < k; i++) total *= 3;
    g_found = (uint64_t *)malloc(total * sizeof(uint64_t));
    g_nfound = 0;
    g_M = M; g_B = B; g_lo = B / 10; g_nd = nd; g_k = k;
    enumerate(0, 0);
    qsort(g_found, (size_t)g_nfound, sizeof(uint64_t), cmp_u64);
    for (int i = 0; i < g_nfound; i++) push(g_found[i]);
    free(g_found);
}

seq_t a(idx_t n) {
    while (cache_len < n && done_digits < 19) {
        done_digits++;
        do_digits(done_digits);
    }
    if (n == 0 || cache_len < n) return (seq_t)0;
    return (seq_t)cache[n - 1];
}
