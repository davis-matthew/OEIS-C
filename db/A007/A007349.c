/*
 * A007349  Primes with both 10 and -10 as primitive root.
 * https://oeis.org/A007349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over primes p and keep those where both 10 and -10 (mod
 *            p) are primitive roots
 * Range:     u8 n <= 11, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

static uint64_t a007349_powmod(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e > 0) {
        if (e & 1) r = (r * b) % m;
        b = (b * b) % m;
        e >>= 1;
    }
    return r;
}

static bool a007349_both_roots(uint64_t p) {
    uint64_t g1 = 10 % p;
    if (g1 == 0) return false;
    uint64_t g2 = p - g1;
    uint64_t pm1 = p - 1;
    uint64_t temp = pm1;
    for (uint64_t q = 2; q * q <= temp; q++) {
        if (temp % q == 0) {
            while (temp % q == 0) temp /= q;
            if (a007349_powmod(g1, pm1 / q, p) == 1) return false;
            if (a007349_powmod(g2, pm1 / q, p) == 1) return false;
        }
    }
    if (temp > 1) {
        if (a007349_powmod(g1, pm1 / temp, p) == 1) return false;
        if (a007349_powmod(g2, pm1 / temp, p) == 1) return false;
    }
    return true;
}

static uint64_t *a007349_cache = NULL;
static uint64_t a007349_len = 0;
static uint64_t a007349_cap = 0;
static uint64_t a007349_next = 3;

seq_t a(idx_t n) {
    while (a007349_len < n) {
        uint64_t p = a007349_next++;
        if (p > 5 && is_prime_u64(p) && a007349_both_roots(p)) {
            if (a007349_len == a007349_cap) {
                uint64_t nc = a007349_cap ? a007349_cap * 2 : 1024;
                a007349_cache = (uint64_t *)realloc(a007349_cache, nc * sizeof(uint64_t));
                a007349_cap = nc;
            }
            a007349_cache[a007349_len++] = p;
        }
    }
    return (seq_t)a007349_cache[n - 1];
}
