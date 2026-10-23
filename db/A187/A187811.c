/*
 * A187811  Numbers having at least one prime factor of form 4*k+3.
 * https://oeis.org/A187811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_prime_factor_3mod4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// Computes OEIS A187811: Numbers having at least one prime factor of form 4*k+3
/* Implementation uses a naive trial‑division factorisation for each new
   candidate.  The algorithm is deterministic, works for any build
   (8/16/32/64/128 bit or arbitrary‑precision seq_t) and keeps a static
   cache so that a(n) can be called in any order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_prime_factor_3mod4.h"

/* Static cache for computed terms. */
static seq_t *seq_cache = NULL;   /* array of seq_t values */
static idx_t  cache_size = 0;      /* number of values currently cached */
static idx_t  cache_cap   = 0;      /* allocated capacity */
static uint64_t next_candidate = 1; /* next integer to test */

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n terms. */
    while (cache_size < n) {
        /* Find the next integer satisfying the property. */
        uint64_t x = next_candidate;
        while (!has_prime_factor_3mod4(x))
            ++x;

        /* Grow cache if necessary. */
        if (cache_size == cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();         /* out of memory – abort safely */
            seq_cache = tmp;
            cache_cap = new_cap;
        }

        seq_cache[cache_size++] = (seq_t)x;
        next_candidate = x + 1;
    }

    /* Return the nth term (1‑based indexing). */
    return seq_cache[n - 1];
}
