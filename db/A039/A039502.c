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
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   none
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
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Test whether a starting value eventually reaches 17. */
static bool reaches17(seq_t start)
{
    seq_t slow = start, fast = start;
    for (;;)
    {
        /* advance slow once */
        if ((slow & 1) == 0)
            slow = slow / 2;
        else
            slow = 3 * slow - 1;
        if (slow == 17)
            return true;

        /* advance fast twice */
        for (int i = 0; i < 2; ++i)
        {
            if ((fast & 1) == 0)
                fast = fast / 2;
            else
                fast = 3 * fast - 1;
            if (fast == 17)
                return true;
        }

        /* If slow and fast meet, a cycle that does not contain 17 has been found. */
        if (slow == fast)
            return false;
    }
}

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
