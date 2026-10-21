/*
 * A097697  Numbers k such that 4*k^2 + 3 is prime.
 * https://oeis.org/A097697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   numbers k such that 4*k^2 + 3 is prime
 * Range:     u8 n <= 59, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin test for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* Bases for deterministic test up to 2^64. */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                    450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, p = a;
        uint64_t exp = d;
        /* binary exponentiation modulo n */
        while (exp) {
            if (exp & 1) x = (x * p) % n;
            p = (p * p) % n;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Cache for computed terms of the sequence. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_k = 0;

/* Append a new term to the cache. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Compute the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset < 1, but harmless */
    while (cache_len < (size_t)n) {
        ++last_k;
        /* Compute 4*k^2 + 3 using 128-bit to avoid overflow. */
        __uint128_t val = (__uint128_t)4 * last_k * last_k + 3;
        if (is_prime_uint64((uint64_t)val))
            cache_append((seq_t)last_k);
    }
    return cache[n - 1];
}
