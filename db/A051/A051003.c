/*
 * A051003  Beastly (or hateful) numbers: numbers containing the string 666 in
 *          their decimal expansion.
 * https://oeis.org/A051003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy, base
 * Formula:   a(n) = the n‑th positive integer whose decimal representation
 *            contains the substring "666" (a(1)=666)
 * Range:     u8 none, u16 n <= 119, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_666
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds the sequence incrementally and caches results for repeated calls. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_666.h"

seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* Number of terms stored. */
    static size_t cache_cap = 0;      /* Allocated capacity of cache. */
    static seq_t next_start = 666;    /* Next integer to test for pattern. */

    /* Grow the cache if needed. */
    if (cache_len < (size_t)n)
    {
        size_t needed = (size_t)n;
        if (needed > cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            if (new_cap < needed)
                new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
            {
                /* Allocation failed – return 0 as a sentinel. */
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate missing terms. */
        while (cache_len < needed)
        {
            seq_t candidate = next_start;
            while (!has_666(candidate))
                candidate++;          /* Skip until we find a number containing "666". */

            cache[cache_len++] = candidate;
            next_start = candidate + 1;
        }
    }

    /* Return the requested term (a is 1‑based). */
    return cache[n - 1];
}
