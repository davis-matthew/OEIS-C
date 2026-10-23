/*
 * A039202  Numbers whose base-11 representation has the same number of 6's
 *          and 9's.
 * https://oeis.org/A039202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   has_equal_6_and_9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose base-11 representation contains an equal number
   of 6's and 9's.  The sequence is stored in ascending order, and
   a(n) returns the n-th element (offset 1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_6_and_9.h"

/* Ensure that the dynamic array 'arr' has capacity for at least 'needed'
   elements.  The capacity is doubled each time it is enlarged. */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed > *cap)
    {
        size_t new_cap = (*cap > 0) ? (*cap << 1) : 64;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *p = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = p;
        *cap = new_cap;
    }
}

/* The sequence function.  It keeps a static cache so that multiple calls
   are fast and independent of call order. */
seq_t a(idx_t n)
{
    /* Static data for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t seen = 0;      /* number of terms stored so far   */
    static seq_t last_checked = 0; /* last number that was examined */

    /* Generate terms until we have at least n of them. */
    while (seen < n)
    {
        if (has_equal_6_and_9(last_checked))
        {
            ++seen; /* we found a new term */
            ensure_capacity(seen, &cache_cap, &cache);
            cache[seen - 1] = last_checked;
        }
        ++last_checked;
    }

    return cache[n - 1];
}
