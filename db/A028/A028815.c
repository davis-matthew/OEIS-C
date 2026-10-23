/*
 * A028815  a(n) = prime(n) + 1 (starting with 1).
 * https://oeis.org/A028815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) + 1, where prime(0)=1 (the artificial unit)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Internal cache of ordinary primes: prime_cache[0] = 2 (prime(1)). */
static uint64_t *prime_cache = NULL;
static size_t cache_len   = 0;
static size_t cache_cap   = 0;
static uint64_t next_candidate = 2;

/* Grow the prime cache to a new capacity. */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);                       /* out‑of‑memory: abort */
    prime_cache = tmp;
    cache_cap   = new_cap;
}

/* Primality test using already cached primes. */
static bool is_prime_candidate(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < cache_len; ++i) {
        uint64_t p = prime_cache[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Ensure that prime_cache contains at least n primes (n≥1). */
static void generate_up_to(idx_t n)
{
    /* We need primes with 1‑based index 1 … n, i.e. n primes. */
    size_t target = (size_t)n;
    while (cache_len < target) {
        if (cache_len + 1 > cache_cap) grow_cache();
        while (!is_prime_candidate(next_candidate)) ++next_candidate;
        prime_cache[cache_len++] = next_candidate;
        ++next_candidate;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Handle the special 0th prime (unit 1). */
    if (n == 0) return (seq_t)2;              /* 1 + 1 */
    /* Ensure primes up to index n are available. */
    generate_up_to(n);
    /* prime_cache[n-1] holds prime(n) for n≥1. */
    return (seq_t)(prime_cache[n - 1] + 1);
}
