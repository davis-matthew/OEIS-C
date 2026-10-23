/*
 * A142064  Primes congruent to 26 mod 33.
 * https://oeis.org/A142064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 325, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime congruent to 26 modulo 33.
   Uses a simple prime test (is_prime) and memoises found terms. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Lazy static cache of found terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t next_k = 1;         /* k such that candidate = 33*k + 26 */
    static bool first = true;

    if (first) {
        /* Initialise cache */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */
        first = false;
    }

    /* Extend cache until we have at least n terms */
    while (cache_size < n) {
        /* Candidate number in the form 33*k + 26 */
        seq_t candidate = (seq_t)(33 * next_k + 26);

        /* Check primality (is_prime works on uint64_t) */
        if (is_prime((idx_t)candidate)) {
            /* Resize cache if needed */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = candidate;
        }
        ++next_k;
    }

    return cache[n - 1];   /* a(1) is the first element of the cache */
}
