/*
 * A031477  Numbers whose base-6 representation has the same number of 0's as
 *          5's.
 * https://oeis.org/A031477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   same_zero_five
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose base‑6 representation has the same number of 0's as 5's */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/same_zero_five.h"

/* Local static cache of already found terms. */
static seq_t *cache = NULL;
static idx_t cache_len   = 0;   /* number of terms stored so far (>=0) */
static idx_t cache_cap   = 0;   /* capacity of the cache array          */
static seq_t next_candidate = 1; /* next integer to test                */

seq_t a(idx_t n)
{
    /* Grow the cache until it contains the n-th term. */
    while (cache_len < n)
    {
        if (same_zero_five(next_candidate))
        {
            /* Ensure enough capacity. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                {
                    /* Out of memory – terminate the program. */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;
    }
    return cache[n - 1];
}
