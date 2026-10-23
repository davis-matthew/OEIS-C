/*
 * A044782  Numbers n such that string 6,9 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_69
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that the decimal representation of n contains
   the substring "69" but the decimal representation of n+1 does not. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/has_69.h"

/* Cached sequence values and generation state */
static seq_t *cache      = NULL;
static size_t cache_size = 0;      /* number of stored terms   */
static size_t cache_cap  = 0;      /* capacity of allocated array */
static seq_t candidate   = 1;      /* next number to test */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for the offset, but safe fallback */

    /* Grow the cache if necessary */
    while (cache_size < (size_t)n)
    {
        /* Ensure we have space for the next term */
        if (cache_cap == 0)       /* first allocation */
        {
            cache_cap = 64;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) return 0; /* allocation failure, unlikely */
        }
        else if (cache_size == cache_cap)
        {
            cache_cap <<= 1;          /* double capacity */
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) return 0;     /* allocation failure */
        }

        if (has_69(candidate) && !has_69(candidate + 1))
        {
            cache[cache_size++] = candidate;
        }
        candidate++;
    }

    return cache[(size_t)n - 1];
}
