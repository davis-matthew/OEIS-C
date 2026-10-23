/*
 * A044555  Numbers n such that string 5,1 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has51
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044555: Numbers n such that the base‑7 representation of n
   contains the substring “51” while that of n+1 does not.  The function
   returns the n‑th term of the sequence (a(1)=36, a(2)=85, …). */

/* The code uses a simple linear scan with memoisation; it is
   suitable for all build configurations and runs in O(n) time. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has51.h"

/* Return the n‑th term of A044555 (1‑based index). */
seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t candidate = 1;   /* next natural number to test */

    /* Initialise the cache on first call. */
    if (cache == NULL) {
        cache_cap = 128;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort(); /* allocation failure */
    }

    /* Extend the cache until it contains at least n terms. */
    while (cache_size < n) {
        if (has51(candidate) && !has51(candidate + (idx_t)1)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) abort(); /* realloc failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the candidate; it is assumed to fit in seq_t. */
            cache[cache_size++] = (seq_t)candidate;
        }
        ++candidate;
    }

    return cache[n - 1];
}
