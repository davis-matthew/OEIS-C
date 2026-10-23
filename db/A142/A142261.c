/*
 * A142261  Primes congruent to 12 mod 43.
 * https://oeis.org/A142261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 151, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142261 – primes congruent to 12 (mod 43).  
   a(n) is the n‑th prime p such that p ≡ 12 (mod 43).  This
   implementation searches the arithmetic progression 12 + 43·k
   using a simple primality routine provided by the utils. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* realloc, free */
#include <stdbool.h>  /* bool */
#include "../utils/is_prime.h"   /* is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* Undefined for n == 0, but return 0 safely. */
    if (n == 0)
        return (seq_t)0;

    /* Static storage for found primes.  */
    static seq_t *cache = NULL;      /* primes found so far */
    static size_t cache_len = 0;      /* number of elements in cache */
    static size_t cache_cap = 0;      /* allocated capacity */

    /* Next candidate in the progression 12 + 43·k (stored in idx_t). */
    static idx_t next_candidate = 12;

    /* Extend the cache until at least n primes are stored. */
    while (cache_len < n) {
        idx_t candidate = next_candidate;
        next_candidate += 43; /* Prepare next number in the progression */

        /* Use the provided primality test. */
        if (is_prime(candidate)) {
            /* Ensure enough space for another prime. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure – clean up and return 0. */
                    free(cache);
                    cache = NULL;
                    cache_cap = 0;
                    cache_len = 0;
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the prime.  It will be cast to seq_t; any overflow
               will simply wrap, which is acceptable for builds with a
               limited seq_t size. */
            cache[cache_len++] = (seq_t)candidate;
        }
    }

    /* Return the requested element (a(1) is at index 0). */
    return cache[n - 1];
}
