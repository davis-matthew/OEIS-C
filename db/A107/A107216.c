/*
 * A107216  Primes of the form 9x^2 + 11y^2.
 * https://oeis.org/A107216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = primes of the form 9*x^2 + 11*y^2, with x,y ∈ ℕ (allowing
 *            0)
 * Range:     u8 n <= 3, u16 n <= 548, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_prime_capacity, insert_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include "../utils/ensure_prime_capacity.h"
#include "../utils/insert_prime.h"

/* 64‑bit modular multiplication using unsigned __int128 */
static uint64_t modmul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* 64‑bit modular exponentiation */
static uint64_t modpow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    a %= mod;
    while (e) {
        if (e & 1)
            res = modmul64(res, a, mod);
        a = modmul64(a, a, mod);
        e >>= 1;
    }
    return res;
}

/* Deterministic Miller–Rabin for 64‑bit integers */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    /* small prime trials */
    static const uint64_t small_primes[] =
        {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    /* write n-1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* deterministic bases for 64‑bit numbers */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = modmul64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Integer square root (floor) for uint64_t */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t r = (uint64_t)sqrt((long double)x);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

/* Check whether v = 9*x^2 + 11*y^2 for some non‑negative integers x,y */
static bool has_representation(uint64_t v)
{
    if (v < 11) return false;          /* minimal possible value */
    /* Enumerate y such that 11*y^2 ≤ v */
    for (uint64_t y = 1;; ++y) {
        /* avoid uint64 overflow when computing 11*y*y */
        if (y > UINT64_MAX / 11) break;
        uint64_t term = 11 * y * y;
        if (term > v) break;
        uint64_t rem = v - term;
        if (rem % 9 != 0) continue;
        uint64_t x2 = rem / 9;
        uint64_t x = isqrt_uint64(x2);
        if (x * x == x2) return true;
    }
    return false;
}

/* Main sequence function (note: external linkage) */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* offset starts at 1 */

    /* static state */
    static uint64_t *prime_list = NULL;
    static size_t prime_cap = 0, prime_cnt = 0;
    static uint64_t last_checked = 0;

    /* Generate primes until we have at least n */
    uint64_t v = last_checked + 1;
    while (prime_cnt < (size_t)n) {
        /* start from 11, skip even numbers (2 cannot be of the required form) */
        if (v < 11) v = 11;
        if (v & 1) {
            if (has_representation(v) && isPrime64(v))
                insert_prime(v, &prime_list, &prime_cnt, &prime_cap);
        }
        ++v;
    }
    last_checked = v - 1;

    return (seq_t)prime_list[n - 1];
}
