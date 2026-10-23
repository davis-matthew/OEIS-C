/*
 * A143811  Number of numbers k<p such that k^(p-1)-1 is divisible by p^2, p =
 *          prime(n).
 * https://oeis.org/A143811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A143811: a(n) = number of integers k < p(n) such that
 * k^(p(n)-1) ≡ 1 (mod p(n)^2), where p(n) is the n-th prime. 
 * Compute by brute‑force modular exponentiation with a small sieve. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/get_prime.h"

#define SIEVE_LIMIT 200000          /* > 10000-th prime (≈ 104729) */
#define CACHE_LIMIT 20001           /* store values up to this index */

/* Global prime sieve */
static uint32_t primes[SIEVE_LIMIT];
static size_t prime_count = 0;
static bool sieve_built = false;

/* Single‑time sieve construction */
static void build_sieve(void) {
    if (sieve_built) return;
    static bool is_comp[SIEVE_LIMIT + 1] = {0};
    for (unsigned int i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_count < SIEVE_LIMIT) {
                primes[prime_count++] = i;
            }
            if ((uint64_t)i * i <= SIEVE_LIMIT) {
                for (unsigned int j = i * i; j <= SIEVE_LIMIT; j += i) {
                    is_comp[j] = true;
                }
            }
        }
    }
    sieve_built = true;
}

/* 64‑bit modular multiplication using 128‑bit intermediate to avoid overflow */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* Binary modular exponentiation: base^exp mod mod */
static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL) res = mul_mod(res, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1ULL;
    }
    return res;
}

/* Cached results for small n */
static seq_t cache_vals[CACHE_LIMIT];
static bool cache_done[CACHE_LIMIT] = {0};

seq_t a(idx_t n) {
    /* Handle caching for small indices */
    if (n < CACHE_LIMIT && cache_done[(size_t)n]) {
        return cache_vals[(size_t)n];
    }

    /* Get the n-th prime (1‑indexed) */
    uint32_t p32 = get_prime((idx_t)n);
    uint64_t p = (uint64_t)p32;
    uint64_t mod = p * p;          /* p^2 fits in 64‑bit */

    /* Ensure sieve is built for prime factor recursion if needed */
    build_sieve();

    /* Count k < p with k^(p-1) ≡ 1 (mod p^2) */
    uint64_t count64 = 0;
    for (uint64_t k = 1; k < p; ++k) {
        if (mod_pow_uint64(k, p - 1, mod) == 1) {
            ++count64;
        }
    }

    /* Convert to seq_t, truncating if necessary for small builds */
    seq_t result = (seq_t)count64;

    /* Store in cache if within limit */
    if (n < CACHE_LIMIT) {
        cache_vals[(size_t)n] = result;
        cache_done[(size_t)n] = true;
    }
    return result;
}
