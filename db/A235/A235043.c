/*
 * A235043  Partial sums of the characteristic function of A091209.
 * https://oeis.org/A235043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k >= 1 with A091209(k) <= n, i.e. the count of
 *            primes p <= n whose binary expansion encodes a
 *            GF(2)[x]-reducible polynomial (sieve-like scan with Rabin
 *            irreducibility test, cached)
 * Range:     u8 n <= 2398, u16 n <= 10007, u32 n <= 10007, u64 n <= 10007,
 *            u128 n <= 10007, big any n
 * Validated: standard, pass, 106 terms through n = 9442 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

static int a235043_deg(uint64_t x) {
    int d = -1;
    while (x) { d++; x >>= 1; }
    return d;
}

static uint64_t a235043_mulmod(uint64_t a, uint64_t b, uint64_t mod, int d) {
    uint64_t res = 0;
    while (b) {
        if (b & 1) res ^= a;
        b >>= 1;
        a <<= 1;
        if ((a >> d) & 1) a ^= mod;
    }
    return res;
}

static uint64_t a235043_pmod(uint64_t a, uint64_t b) {
    int db = a235043_deg(b);
    int da = a235043_deg(a);
    while (da >= db) {
        a ^= b << (da - db);
        da = a235043_deg(a);
    }
    return a;
}

static uint64_t a235043_gcd(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t r = a235043_pmod(a, b);
        a = b;
        b = r;
    }
    return a;
}

static uint64_t a235043_xpow2k(unsigned k, uint64_t mod, int d) {
    uint64_t r = 2;
    for (unsigned i = 0; i < k; ++i) r = a235043_mulmod(r, r, mod, d);
    return r;
}

static bool a235043_reducible(uint64_t poly) {
    if (poly < 4) return false;
    int d = a235043_deg(poly);
    unsigned t = (unsigned)d;
    for (unsigned q = 2; q * q <= t || t > 1; ++q) {
        if (q * q > t) q = t;
        if (t % q == 0) {
            while (t % q == 0) t /= q;
            uint64_t y = a235043_xpow2k((unsigned)d / q, poly, d);
            if (a235043_gcd(poly, y ^ 2) != 1) return true;
        }
        if (t <= 1) break;
    }
    return (a235043_xpow2k((unsigned)d, poly, d) ^ 2) != 0;
}

static uint64_t *a235043_list = NULL;
static size_t a235043_cnt = 0, a235043_cap = 0;
static uint64_t a235043_scanned = 2; /* all numbers <= scanned examined */

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    while (a235043_scanned < N) {
        uint64_t p = ++a235043_scanned;
        if ((p & 1) == 0) continue;
        if (!is_prime_u64(p)) continue;
        if (a235043_reducible(p)) {
            if (a235043_cnt == a235043_cap) {
                size_t nc = a235043_cap ? a235043_cap * 2 : 64;
                uint64_t *t = (uint64_t *)realloc(a235043_list, nc * sizeof(uint64_t));
                if (!t) exit(EXIT_FAILURE);
                a235043_list = t;
                a235043_cap = nc;
            }
            a235043_list[a235043_cnt++] = p;
        }
    }
    size_t lo = 0, hi = a235043_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (a235043_list[mid] <= N) lo = mid + 1; else hi = mid;
    }
    return (seq_t)lo;
}
