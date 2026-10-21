/*
 * A059241  Primes p such that x^11 = 2 has no solution mod p.
 * https://oeis.org/A059241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 593, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the primes p for which the equation x^11 = 2 has no solution modulo p.
   These are exactly the primes p ≡ 1 (mod 11) with 2^{(p−1)/11} ≠ 1 (mod p). */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"          /* mod_pow(seq_t base, uint64_t exp, seq_t mod) */

static const uint64_t SIEVE_LIMIT = 6000000;   /* sufficient for n ≤ 10000 */

static uint32_t *primes = NULL;    /* array of all primes up to SIEVE_LIMIT */
static size_t prime_count = 0;

static seq_t *cache = NULL;        /* cache of qualifying primes */
static size_t cache_size = 0;      /* number of elements currently in cache */
static size_t cache_cap  = 0;      /* allocated capacity of cache */

static size_t processed_prime_idx = 0;   /* index of next prime to test */
static bool initialized = false;

/* Simple sieve of Eratosthenes. */
static void build_prime_list(void)
{
    bool *is_comp = (bool*)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_comp) return;          /* unlikely, but keep safe */

    for (uint64_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (!is_comp[i])
            for (uint64_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_comp[j] = true;

    size_t cnt = 0;
    for (uint64_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_comp[i]) ++cnt;

    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!primes) { free(is_comp); return; }

    for (uint64_t i = 2, idx = 0; i <= SIEVE_LIMIT; ++i)
        if (!is_comp[i]) primes[idx++] = (uint32_t)i;

    prime_count = cnt;
    free(is_comp);
}

/* Test whether prime p satisfies the condition. */
static bool satisfies(uint32_t p)
{
    if (p % 11 != 1) return false;
    seq_t val = mod_pow((seq_t)2, (uint64_t)(p - 1) / 11, (seq_t)p);
    return val != 1;
}

/* Grow the cache until it contains at least n qualifying primes. */
static void ensure_cache_size(idx_t n)
{
    if (cache_size >= (size_t)n) return;

    if (!initialized) { build_prime_list(); initialized = true; }

    /* Expand cache capacity if needed. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;           /* unlikely, but keep safe */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Scan remaining primes and add qualifying ones. */
    for (; processed_prime_idx < prime_count && cache_size < (size_t)n; ++processed_prime_idx) {
        uint32_t p = primes[processed_prime_idx];
        if (satisfies(p)) {
            cache[cache_size++] = (seq_t)p;
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */
    ensure_cache_size(n);
    return cache[(size_t)n - 1];
}
