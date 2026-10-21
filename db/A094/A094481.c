/*
 * A094481  Duplicate of A045637.
 * https://oeis.org/A094481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 14, i32 n <= 14, i64 n <= 14, i128 n <= 14,
 *            big any n
 * Helpers:   rebuild_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes of the form q² + 4 where q is prime */
/* a(n) returns the nth such prime, with offset 1. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/rebuild_primes.h"

/* Deterministic Miller–Rabin for 64‑bit integers */
static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)((__uint128_t)a * b % m);
}

static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    a %= m;
    while (e) {
        if (e & 1ULL) r = mod_mul(r, a, m);
        a = mod_mul(a, a, m);
        e >>= 1;
    }
    return r;
}

static int is_prime_uint64(uint64_t n)
{
    if (n < 2) return 0;
    static const uint64_t small_primes[] =
        { 2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n-1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }
    static const uint64_t bases[] =
        { 2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n-1) continue;
        int composite = 1;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n-1) { composite = 0; break; }
        }
        if (composite) return 0;
    }
    return 1;
}

/* Global state --------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_size = 0, cache_cap = 0;
static uint32_t *prime_cache = NULL;
static size_t prime_cache_size = 0;
static uint32_t current_limit = 0;
static size_t processed_prime_index = 0;
static int init_done = 0;

/* Ensure cache has enough capacity */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t newcap = cache_cap ? cache_cap : 64;
    while (newcap < needed) newcap <<= 1;
    cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
    cache_cap = newcap;
}

/* Extend cache to contain at least n terms */
static void extend_cache(idx_t n)
{
    if (cache_size >= (size_t)n) return;
    if (!init_done) {
        init_done = 1;
        current_limit = 1000000U;           /* start with 1 M */
        rebuild_primes(current_limit, &prime_cache, &prime_cache_size);
    }
    while (cache_size < (size_t)n) {
        /* Process remaining primes in current buffer */
        while (processed_prime_index < prime_cache_size && cache_size < (size_t)n) {
            uint32_t q = prime_cache[processed_prime_index++];
            uint64_t p = (uint64_t)q * q + 4ULL;
            if (is_prime_uint64(p)) {
                grow_cache(cache_size + 1);
                cache[cache_size++] = (seq_t)p;
            }
        }
        if (cache_size >= (size_t)n) break;
        /* Not enough terms yet – double limit and rebuild */
        current_limit <<= 1;
        rebuild_primes(current_limit, &prime_cache, &prime_cache_size);
        processed_prime_index = 0;  /* start at beginning of new list */
    }
}

/* Public function --------------------------------------------------- */
seq_t a(idx_t n)
{
    extend_cache(n);
    return cache[(size_t)n - 1];
}
