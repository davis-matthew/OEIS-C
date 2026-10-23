/*
 * A039502  Iterations of "n->n/2 if n even, n->3n-1 if n odd" (A001281)
 *          starting at these numbers reach 17.
 * https://oeis.org/A039502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   reaches17, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence definition: a(n) is the n-th positive integer that eventually
   reaches 17 under the mapping n → n/2 if n even, n → 3n−1 if n odd
   (OEIS A039502).  The function generates this list on the fly.
   It uses memoization to store the found terms and expands the array
   as needed.  For the arbitrary‑precision build it relies on the helper
   functions seq_alloc and seq_realloc to allocate and reallocate a
   contiguous array of seq_t objects safely in C++.
*/

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/reaches17.h"

/* External linkage is required by the build system. */
seq_t a(idx_t n)   /* 1‑based index */
{
    /* Static cache of numbers that reach 17. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0; /* number of elements stored */
    static size_t cache_cap  = 0; /* allocated capacity */
    static seq_t next_to_check = 1; /* next integer to test */

    /* Allocate or grow the cache as needed. */
    if (cache_cap == 0)
    {
        cache_cap = 16;
        cache = seq_alloc(cache_cap);
    }
    else if (cache_size + 1 > cache_cap)
    {
        cache_cap *= 2;
        cache = seq_realloc(cache, cache_cap / 2, cache_cap);
    }

    /* Extend the cache until it contains at least n terms. */
    while (cache_size < (size_t)n)
    {
        /* The very first term in the sequence is 17 itself. */
        if (next_to_check == 17)
        {
            cache[cache_size++] = 17;
            ++next_to_check;
        }
        else if (reaches17(next_to_check))
        {
            cache[cache_size++] = next_to_check;
            ++next_to_check;
        }
        else
        {
            ++next_to_check;
        }
    }

    return cache[n - 1];
}
