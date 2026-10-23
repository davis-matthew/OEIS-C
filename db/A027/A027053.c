/*
 * A027053  a(n) = T(n,n+2), T given by A027052.
 * https://oeis.org/A027053
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + 4; a(2)=1, a(3)=4, a(4)=9.
 *            Offset: 2
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   alloc_new, free_new
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_new.h"
#include "../utils/free_new.h"

/* Dynamic allocation helpers that work in C and C++ */
#ifdef __cplusplus

#else

#endif

/* Return the n‑th term of A027053 (n ≥ 2).  The cache is grown on first
   access to n and never shrinks.  The sequence is defined by the
   recurrence:
         a(n) = a(n-1) + a(n-2) + a(n-3) + 4,   for n ≥ 5
   with the starting values a(2)=1, a(3)=4, a(4)=9. */
seq_t a(idx_t n)
{
    /* Static cache of computed values.  cache_size is the number of
       allocated entries (valid indices 0 .. cache_size-1). */
    static size_t cache_size = 0;
    static seq_t *cache      = NULL;

    /* Ensure array covers index n. */
    if ((size_t)n + 1 > cache_size)
    {
        /* Choose a new, larger size – at least 8 to cover the initial
           three values.  Grow geometrically to keep amortised O(1)
           operations. */
        size_t new_size = cache_size ? cache_size * 2 : 8;
        while (new_size <= (size_t)n)
            new_size <<= 1;

        seq_t *new_cache = alloc_new(new_size);
        if (cache)
        {
            for (size_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
            free_new(cache);
        }

        /* Initialise all newly allocated entries.  Indices < 2 are
           unused; set them to 0.  Indices 2,3,4 are the seed values;
           subsequent entries follow the recurrence. */
        for (size_t i = cache_size; i < new_size; ++i)
        {
            if (i < 2)
                new_cache[i] = (seq_t)0;
            else if (i == 2)
                new_cache[i] = (seq_t)1;
            else if (i == 3)
                new_cache[i] = (seq_t)4;
            else if (i == 4)
                new_cache[i] = (seq_t)9;
            else
                new_cache[i] = new_cache[i-1] + new_cache[i-2] + new_cache[i-3] + (seq_t)4;
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
