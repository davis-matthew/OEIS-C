/*
 * A142833  Primes congruent to 35 mod 61.
 * https://oeis.org/A142833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 110, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 35 mod 61 (OEIS A142833) */
/* a(n) is the nth prime p such that p ≡ 35 (mod 61), offset = 1 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static uint64_t *prime_cache = NULL;   /* array of found primes (uint64_t) */
static size_t  cache_cap   = 0;       /* allocated capacity of prime_cache */
static size_t  cache_len   = 0;       /* number of primes stored */
static uint64_t next_k      = 2;      /* next k to test (p = 61*k + 35) */

/* Append a prime to the cache, reallocating if necessary. */
static void add_prime(uint64_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
        if (!tmp) abort();           /* out of memory – terminate */
        prime_cache = tmp;
        cache_cap   = new_cap;
    }
    prime_cache[cache_len++] = val;
}

/* Ensure that the cache contains at least n primes (n ≥ 1). */
static void ensure_cache_size(idx_t n)
{
    while (cache_len < n) {
        /* Find the next prime of the form 61*k + 35. */
        while (1) {
            uint64_t cand = 61ULL * next_k + 35ULL;
            if (is_prime((idx_t)cand)) {
                add_prime(cand);
                break;  /* found a prime, exit inner loop */
            }
            ++next_k;         /* test the next candidate */
        }
        ++next_k;             /* skip the prime just found for the next search */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence offset is 1 */
    ensure_cache_size(n);
    return (seq_t)prime_cache[n - 1];
}
