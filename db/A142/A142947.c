/*
 * A142947  Primes congruent to 49 mod 64.
 * https://oeis.org/A142947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 203, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142947: primes p ≡ 49 (mod 64).  This function
   returns the n‑th term (a(1) = 113) by generating candidates
   49 + 64*k and testing them for primality with the helper
   is_prime from utils.  Results are cached between calls. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/is_prime.h"   /* provides: bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    /* Static cache of previously found primes of the required form. */
    static seq_t *cache        = NULL;
    static size_t cache_cap    = 0;   /* allocated capacity */
    static size_t cache_len    = 0;   /* number of primes stored */

    /* Next k-value for the arithmetic progression 49 + 64*k. */
    static idx_t next_k        = 1;   /* 49 + 64*1 = 113 (the first term) */

    /* Allocate the initial cache when needed. */
    if (cache_cap == 0) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Ensure we have at least n primes in the cache. */
    while (cache_len < n) {
        idx_t cand = (idx_t)49 + (idx_t)64 * next_k;
        next_k++;

        /* Use the provided primality test. */
        if (is_prime(cand)) {
            /* Grow the cache if it is full. */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {  /* allocation failure – fallback to 0 */
                    free(cache);
                    cache = NULL;
                    cache_cap = 0;
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }

    return cache[(size_t)(n - 1)];
}
