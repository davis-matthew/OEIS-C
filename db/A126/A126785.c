/*
 * A126785  Numbers k such that 10*k + 11 is prime.
 * https://oeis.org/A126785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth integer k such that 10*k + 11 is prime.  a(1)=0. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Returns the nth element of sequence A126785, starting at a(1). */
seq_t a(idx_t n)
{
    /* Static cache of computed ks, growing as needed. */
    static seq_t *cache = NULL;      /* array of ks */
    static size_t cache_size = 0;    /* number of stored ks       */
    static size_t cache_cap  = 0;    /* allocated capacity        */
    static idx_t candidate_k = 0;    /* next k to test            */

    if (n == 0) return 0;           /* undefined, but keep safety */

    /* Ensure the cache contains at least n elements. */
    while (cache_size < n) {
        /* Find next k for which 10*k+11 is prime. */
        for (;; ++candidate_k) {
            idx_t test = 10 * candidate_k + 11;
            if (is_prime(test))
                break;                 /* candidate_k satisfies condition */
        }

        /* Grow array if necessary. */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failed – abort gracefully. */
                exit(EXIT_FAILURE);
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the found k and advance to the next candidate. */
        cache[cache_size++] = (seq_t)candidate_k;
        ++candidate_k;        /* skip the used k for the next search   */
    }

    return cache[n - 1];      /* 1‑based indexing: a(1) is first element */
}
