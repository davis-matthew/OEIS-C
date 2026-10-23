/*
 * A142002  Primes congruent to 26 mod 29.
 * https://oeis.org/A142002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 229, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = nth prime congruent to 26 (mod 29) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* The utility functions need to be included */
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_realloc.h"

/* The function required by the framework */
seq_t a(idx_t n)
{
    /* Static cache for the primes */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Candidate numbers that are ≡ 26 (mod 29) */
    static idx_t candidate = 26;  /* will be increased before first test */

    /* Grow the cache as needed */
    if (cache == NULL) {
        cache_cap = 8;
        cache = seq_realloc(NULL, 0, cache_cap);
        if (!cache) return 0;  /* allocation failure */
    }

    /* Find more primes until we have at least n of them */
    while (cache_len < n) {
        candidate += 29;                    /* next number ≡ 26 (mod 29) */
        if (isPrimeGeneric((seq_t)candidate)) {
            /* Ensure enough capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) return 0;        /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)candidate;
        }
    }

    return cache[n - 1];
}
