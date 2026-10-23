/*
 * A155937  Numbers n such that 13*n + 8 is a prime.
 * https://oeis.org/A155937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n such that 13*n + 8 is a prime */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* Helper function: test if x is prime using a small prime table up to 2000. */
static inline bool is_prime_uint64(uint64_t x, const uint32_t *primes, size_t primes_cnt)
{
    if (x < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > x) break;
        if (x % p == 0) return (x == p);
    }
    return true;
}

/* Compute the nth term of the sequence A155937. */
seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of entries stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static uint64_t next_candidate = 1; /* next integer to test for being in the sequence */

    /* Simple sieve to generate primes up to 2000 once */
    static bool primes_init = false;
    static uint32_t primes[305]; /* there are 305 primes <=2000 */
    static size_t primes_cnt = 0;

    if (!primes_init) {
        /* Build list of primes <=2000 */
        bool is_prime[2001];
        for (int i = 0; i <= 2000; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (int i = 2; i * i <= 2000; ++i) {
            if (is_prime[i]) {
                for (int j = i * i; j <= 2000; j += i)
                    is_prime[j] = false;
            }
        }
        for (int i = 2; i <= 2000; ++i)
            if (is_prime[i]) primes[primes_cnt++] = (uint32_t)i;
        primes_init = true;
    }

    /* Ensure we have enough terms cached */
    while (cache_len < n) {
        uint64_t m = next_candidate++;
        uint64_t val = 13 * m + 8;
        /* Test primality using the precomputed prime list */
        if (is_prime_uint64(val, primes, primes_cnt)) {
            /* Append to cache */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *newp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!newp) {
                    /* Allocation failed – return 0 as an error indicator. */
                    free(cache);
                    cache = NULL;
                    cache_len = cache_cap = 0;
                    return 0;
                }
                cache = newp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)m;
        }
    }

    return cache[n - 1];
}
