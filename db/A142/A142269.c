/*
 * A142269  Primes congruent to 20 mod 43.
 * https://oeis.org/A142269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime congruent to 20 (mod 43)
 * Range:     u8 n <= 1, u16 n <= 152, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search successive integers of the form 20+43*k and keep the ones that are
   prime.  A small dynamic array caches the found primes so that calls with
   arbitrary order of n remain fast. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* static cache holding the primes found so far */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_size = 0;
    static idx_t next_candidate = 20;  /* start at 20 (≡20 mod 43) */

    /* first time we get called: allocate initial storage */
    if (!cache) {
        cache_cap = 256;                                 /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                            /* out of memory */
    }

    /* Grow the list until it contains at least n primes */
    while (cache_size < n) {
        if (is_prime(next_candidate)) {
            /* grow array if necessary */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp) return 0;  /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
        next_candidate += 43;   /* next integer ≡20 (mod 43) */
    }

    return cache[(size_t)(n - 1)];
}
