/*
 * A044232  Numbers k such that string 5,5 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1010, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has55_oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the base‑8 representation of k contains the
   substring "55" but the representation of k‑1 does NOT.  The OEIS
   sequence is A044232. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has55_oct.h"

/* Cache state */
static seq_t *cache = NULL;          /* Cached terms */
static size_t cache_cap = 0;          /* Allocated capacity */
static idx_t cache_size = 0;          /* Number of cached terms */
static uint64_t next_search = 1;      /* Next integer to test */

/* Ensure space for a new term in the cache */
static void ensure_capacity(void)
{
    if (cache_size >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        seq_t *newptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newptr)
            abort();                 /* allocation failure is fatal */
        cache = newptr;
        cache_cap = new_cap;
    }
}

/* Main function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;             /* undefined for n == 0 */

    /* Lazy initialization of cache */
    if (!cache) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
    }

    /* Extend cache until we have at least n terms */
    while (cache_size < n) {
        /* Find next suitable k */
        while (true) {
            if (has55_oct(next_search) &&
                !has55_oct(next_search - 1)) {
                /* Store it */
                ensure_capacity();
                cache[cache_size++] = (seq_t)next_search;
                next_search++;          /* advance past this k */
                break;
            }
            next_search++;
        }
    }

    return cache[n - 1];
}
