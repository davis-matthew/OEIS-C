/*
 * A139639  Numbers n such that 168n+31 is prime.
 * https://oeis.org/A139639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer m such that 168*m + 31 is prime
 * Range:     u8 n <= 2, u16 n <= 134, u32 n <= 135, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */

seq_t a(idx_t n)
{
    /* cache of computed terms: a(1) is at cache[0] */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;      /* number of terms currently stored */
    static size_t cache_cap = 0;      /* current capacity of the cache */
    static idx_t  next_candidate = 0; /* next integer m to test */

    /* initialise the cache on first call */
    if (cache == NULL) {
        cache_cap = 256;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            abort();
    }

    /* extend the cache until it contains at least n terms */
    while (cache_len < n) {
        /* compute 168 * next_candidate + 31 as seq_t */
        seq_t val = ((seq_t)168 * (seq_t)next_candidate) + (seq_t)31;

        /* test primality using the big‑integer routine */
        if (is_prime_big(val)) {
            /* enlarge the cache if needed */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (new_cache == NULL)
                    abort();
                cache = new_cache;
            }
            /* store the candidate in the cache */
            cache[cache_len] = (seq_t)next_candidate;
            ++cache_len;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
