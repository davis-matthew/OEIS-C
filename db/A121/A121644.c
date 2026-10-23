/*
 * A121644  Slowest increasing sequence with squarefree cumulative sums.
 * https://oeis.org/A121644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 293, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   init_global_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A121644: Slowest increasing sequence with squarefree cumulative sums */
/* a(n) is the smallest integer > a(n-1) such that the sum of the first n terms
   is squarefree.  The sequence begins 1,2,3,4,5,6,8,9,13,14,... */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/init_global_primes.h"

static bool is_squarefree(seq_t v, const uint32_t *primes, size_t prime_len)
{
    if (v <= 1) return true;

    for (size_t i = 0; i < prime_len; ++i) {
        seq_t p  = (seq_t)primes[i];
        seq_t p2 = p * p;
        if (p2 > v) break;
        if (v % p2 == 0) return false;
    }
    return true;
}

/* External function required by the build */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 0;
    static seq_t cumulative_sum = 0;

    static uint32_t *primes = NULL;
    static size_t prime_len = 0;
    static bool primes_initialized = false;

    if (!primes_initialized) {
        init_global_primes(&primes, &prime_len);
        primes_initialized = true;
    }

    /* Allocate cache if necessary */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Compute terms up to n if they are not yet known */
    while (last_computed < n) {
        seq_t prev = (last_computed == 0) ? 0 : cache[last_computed - 1];
        seq_t candidate = prev + 1;

        /* Search for the next valid term */
        for (;;) {
            seq_t new_sum = cumulative_sum + candidate;
            if (is_squarefree(new_sum, primes, prime_len))
                break;
            ++candidate;
        }

        /* Grow cache if needed */
        if (cache_cap <= last_computed) {
            size_t new_cap = cache_cap * 2;
            if (new_cap == 0) new_cap = 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);  /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[last_computed] = candidate;
        cumulative_sum += candidate;
        ++last_computed;
    }

    return cache[n - 1];
}
