/*
 * A089443  Primes p such that 12*p + 13 is prime.
 * https://oeis.org/A089443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1451, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for primes p such that 12*p + 13 is prime.
   For each n ≥ 1, a(n) is the n‑th prime p with that property. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t  last_checked = 1;     /* last integer examined */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* out of memory – caller will handle */
    }

    /* extend cache until it contains at least n terms */
    while (cache_len < n) {
        idx_t candidate = last_checked + 1;
        last_checked = candidate;

        /* candidate must be prime */
        if (!is_prime(candidate))
            continue;

        /* check if 12*candidate + 13 is prime */
        uint64_t test = 12ULL * (uint64_t)candidate + 13ULL;
        if (!is_prime((idx_t)test))
            continue;

        /* add to cache */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;   /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)candidate;
    }

    return cache[n-1];
}
