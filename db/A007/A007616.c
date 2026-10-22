/*
 * A007616  Palindromic reflectable primes.
 * https://oeis.org/A007616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 114, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: enumerate odd-length palindromes (plus 11) whose digits are in {0,1,3,8},
   in increasing order, and keep the primes (Miller-Rabin). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128;

static u128 *a7616_terms = 0;
static size_t a7616_count = 0, a7616_cap = 0;
static int a7616_nextlen = 1;

static void a7616_push(u128 v) {
    if (a7616_count == a7616_cap) {
        a7616_cap = a7616_cap ? 2 * a7616_cap : 1024;
        a7616_terms = (u128 *)realloc(a7616_terms, a7616_cap * sizeof(u128));
    }
    a7616_terms[a7616_count++] = v;
}

static u128 a7616_mulmod(u128 a, u128 b, u128 m) {
    if (m >> 64 == 0) return (a * b) % m;
    u128 r = 0;
    a %= m;
    while (b) {
        if (b & 1) {
            r = (r >= m - a) ? r - (m - a) : r + a;
        }
        b >>= 1;
        if (b) a = (a >= m - a) ? a - (m - a) : a + a;
    }
    return r;
}

static u128 a7616_powmod(u128 b, u128 e, u128 m) {
    u128 r = 1;
    b %= m;
    while (e) {
        if (e & 1) r = a7616_mulmod(r, b, m);
        e >>= 1;
        if (e) b = a7616_mulmod(b, b, m);
    }
    return r;
}

static bool a7616_isprime(u128 n) {
    static const unsigned sp[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71};
    int i, j;
    if (n < 2) return false;
    for (i = 0; i < 20; i++) {
        if (n == sp[i]) return true;
        if (n % sp[i] == 0) return false;
    }
    if (n < 71 * 71) return true;
    u128 d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 13; i++) {
        u128 x = a7616_powmod(sp[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (j = 1; j < s; j++) {
            x = a7616_mulmod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static void a7616_gen_length(int L) {
    static const int dig[4] = {0, 1, 3, 8};
    if (L == 1) { a7616_push(3); return; }
    if (L == 2) { a7616_push(11); return; }
    if (L % 2 == 0) return;
    int k = (L - 1) / 2;
    uint64_t total = (uint64_t)2 << (2 * k);
    uint64_t idx;
    int h[64];
    int i;
    for (idx = 0; idx < total; idx++) {
        uint64_t t = idx;
        for (i = k; i >= 1; i--) { h[i] = dig[t & 3]; t >>= 2; }
        h[0] = dig[1 + (int)t];
        u128 v = 0;
        for (i = 0; i <= k; i++) v = v * 10 + (u128)h[i];
        for (i = k - 1; i >= 0; i--) v = v * 10 + (u128)h[i];
        if (a7616_isprime(v)) a7616_push(v);
    }
}

seq_t a(idx_t n) {
    while (a7616_count < (size_t)n) {
        a7616_gen_length(a7616_nextlen++);
    }
    return u128_to_seq(a7616_terms[n - 1]);
}
