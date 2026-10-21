/*
 * A052126  a(1) = 1; for n>1, a(n)=n/(greatest prime dividing n).
 * https://oeis.org/A052126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n / (largest prime factor of n)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd_uint64.h"

/* modular multiplication modulo 'mod' – works for 64‑bit values */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* modular exponentiation (a^e) mod mod */
static inline uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* GCD for 64‑bit values */

/* Deterministic Miller–Rabin primality test for 64‑bit integers */
static inline bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    /* small prime trial division */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
        17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* bases covering all 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Pollard‑Rho algorithm to find a nontrivial factor */
static uint64_t pollard_rho(uint64_t n) {
    if ((n & 1ULL) == 0ULL) return 2ULL;
    static uint64_t c = 1ULL; /* constant for f(x) = x^2 + c */
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mul_mod(x, x, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n; /* y = f(f(y)) */
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_uint64(diff, n);
        }
        if (d != n) return d;
        ++c;
    }
}

/* Recursively find the largest prime factor of n */
static uint64_t largest_prime_factor(uint64_t n) {
    if (n < 2) return n;
    if (is_prime_uint64(n)) return n;
    uint64_t d = pollard_rho(n);
    uint64_t a = largest_prime_factor(d);
    uint64_t b = largest_prime_factor(n / d);
    return a > b ? a : b;
}

/* OEIS A052126 – a(n) = n / (largest prime factor of n) */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;            /* a(1) = 1 */
    uint64_t m = (uint64_t)n;
    uint64_t gpf = largest_prime_factor(m);
    return (seq_t)(m / gpf);
}
