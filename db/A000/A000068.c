/*
 * A000068  Numbers k such that k^4 + 1 is prime.
 * https://oeis.org/A000068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: a(n) is the n-th k >= 1 with k^4 + 1 prime (trial
 *            division by small p == 1 mod 8, then Miller-Rabin)
 * Range:     u8 n <= 38, u16 n <= 4321, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 u128_t;

static int bitlen128(u128_t m)
{
    int L = 0;
    while (m) { L++; m >>= 1; }
    return L;
}

static u128_t mulmod128(u128_t a, u128_t b, u128_t m, int L)
{
    if (L <= 63) return a * b % m;
    int c = 127 - L;
    int nch = (L + c - 1) / c;
    u128_t mask = (((u128_t)1) << c) - 1;
    u128_t r = 0;
    for (int i = nch - 1; i >= 0; i--) {
        u128_t ch = (b >> (i * c)) & mask;
        r = ((r << c) + a * ch) % m;
    }
    return r;
}

static u128_t powmod128(u128_t b, u128_t e, u128_t m, int L)
{
    u128_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = mulmod128(r, b, m, L);
        e >>= 1;
        if (e) b = mulmod128(b, b, m, L);
    }
    return r;
}

static bool mr_prime128(u128_t n)
{
    static const unsigned bases[13] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41};
    if (n < 2) return false;
    for (int i = 0; i < 13; i++) {
        if (n == bases[i]) return true;
        if (n % bases[i] == 0) return false;
    }
    int L = bitlen128(n);
    u128_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 13; i++) {
        u128_t x = powmod128(bases[i], d, n, L);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int j = 1; j < s; j++) {
            x = mulmod128(x, x, n, L);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

#define SMALL_LIM 4000
static uint64_t small_p[SMALL_LIM];
static int n_small = -1;

static void init_small(void)
{
    static bool comp[SMALL_LIM + 1];
    n_small = 0;
    for (int i = 2; i <= SMALL_LIM; i++) {
        if (comp[i]) continue;
        for (int j = i * i; j <= SMALL_LIM; j += i) comp[j] = true;
        if (i % 8 == 1) small_p[n_small++] = (uint64_t)i;
    }
}

static bool is_k_term(uint64_t k)
{
    if (k == 1) return true;
    if (k & 1) return false;
    u128_t N = (u128_t)k * k;
    N = N * N + 1;
    for (int i = 0; i < n_small; i++) {
        uint64_t p = small_p[i];
        uint64_t r = k % p;
        r = r * r % p;
        r = r * r % p;
        if ((r + 1) % p == 0) return N == (u128_t)p;
    }
    return mr_prime128(N);
}

static uint64_t *ks = NULL;
static size_t cnt = 0, cap = 0;
static uint64_t next_k = 1;

seq_t a(idx_t n)
{
    if (n_small < 0) init_small();
    while (cnt < n) {
        uint64_t k = next_k;
        next_k = (k == 1) ? 2 : k + 2;
        if (is_k_term(k)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                ks = (uint64_t *)realloc(ks, cap * sizeof(uint64_t));
            }
            ks[cnt++] = k;
        }
    }
    return (seq_t)ks[n - 1];
}
