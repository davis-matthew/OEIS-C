/*
 * A176897  Primes p such that (p+1)/2 is not a prime.
 * https://oeis.org/A176897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 6056, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A176897: primes p such that (p+1)/2 is not prime. */
/* This implementation builds the sequence incrementally, caching
   already found terms.  It uses the provided is_prime(idx_t) helper
   from the build environment. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* Return the nth term (1‑based) of OEIS sequence A176897. */
seq_t a(idx_t n)
{
    /* Cached terms of the sequence up to the largest index that has
       been requested so far. */
    static seq_t *cache = NULL;    /* array of seq_t values */
    static size_t cache_len = 0;   /* number of elements stored */
    static size_t cache_cap = 0;   /* current allocation capacity */
    static idx_t next_candidate = 3; /* next odd integer to test for
                                        being a suitable prime */

    /* Ensure that the first term (2) is present in the cache. */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)2;
        cache_len = 1;
    }

    /* Generate additional terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Test next_candidate for primality. */
        if (is_prime(next_candidate)) {
            /* For an odd prime p, compute m = (p+1)/2. */
            idx_t half = (next_candidate >> 1) + 1; /* (p+1)/2 */

            /* If m is NOT prime, then p is a term of the sequence. */
            if (!is_prime(half)) {
                /* Grow the cache if necessary. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next_candidate;
            }
        }
        /* Move to the next odd integer. */
        next_candidate += 2;
    }

    return cache[n - 1];
}
