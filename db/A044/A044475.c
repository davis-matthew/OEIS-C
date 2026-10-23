/*
 * A044475  Numbers n such that string 3,2 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has32_base5, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Procedure: numbers n such that the base‑5 representation of n contains
   the substring "32" but the representation of n+1 does not. */
/* Dependencies: seq_t and idx_t are supplied by the build environment.
   The function uses seq_alloc and seq_realloc from the utils package. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/has32_base5.h"

/* Main sequence generator with memoization.  The cache stores the found
   elements of the sequence. */
seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;  /* contiguous array of terms */
    static size_t cache_size = 0;    /* number of terms stored */
    static size_t cache_cap   = 0;    /* allocated capacity */
    static idx_t last_checked = 0;   /* next candidate integer to test */

    /* Ensure n is at least 1 (the sequence starts at offset 1). */
    if (n < 1)
        return (seq_t)0;  /* undefined for n < 1, return 0 */

    /* Initialize the first candidate to 1 (the first positive integer). */
    if (last_checked == 0)
        last_checked = 1;

    /* Grow the cache until we have at least n terms. */
    while (cache_size < (size_t)n)
    {
        seq_t candidate = (seq_t)last_checked;

        if (has32_base5(candidate) && !has32_base5(candidate + (seq_t)1))
        {
            /* Grow the cache if necessary. */
            if (cache_size >= cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16U : cache_cap << 1U;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (tmp == NULL)
                    exit(EXIT_FAILURE);  /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = candidate;
        }

        last_checked++;
    }

    return cache[n - 1];  /* OEIS uses 1‑based indexing */
}
