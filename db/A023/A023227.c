/*
 * A023227  Primes p such that 7*p + 10 is also prime.
 * https://oeis.org/A023227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 921 (2026-11-03)
 * Helpers:   is_prime, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth prime p such that 7*p+10 is also prime. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    /* Static cache to store found primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    /* Last prime examined; start below the first prime to make next_prime work */
    static uint64_t last_checked = 2;

    /* a(1) is the first term; n must be >= 1 for valid calls.
       If n == 0 we return 0 as a placeholder. */
    if (n == 0)
        return 0;

    /* Return cached value if already computed */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* Generate additional primes until we have at least n elements */
    while (cache_size < (size_t)n) {
        uint64_t prime = next_prime(last_checked);
        last_checked = prime;

        /* Compute 7*prime + 10 and test its primality */
        uint64_t val = 7ull * prime + 10ull;
        if (is_prime(val)) {
            /* Ensure the cache has enough capacity and store the prime */
            ensure_cache(cache_size + 1, &cache, &cache_cap);
            cache[cache_size++] = (seq_t)prime;
        }
    }

    return cache[n - 1];
}
