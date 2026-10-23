/*
 * A043929  Numbers k such that 4 and 5 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_adjacent_45_in_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043929: numbers k such that 4 and 5 occur juxtaposed in the
   base‑7 representation of k but not of k+1. This implementation
   generates the desired numbers by linear scan, caching the results
   for subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_45_in_base7.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached sequence values           */
    static size_t size = 0;         /* number of cached elements        */
    static size_t cap  = 0;         /* current array capacity           */
    static seq_t next_k = 0;        /* next integer to test             */

    /* Ensure the cache holds at least n elements. */
    if (size < n)
    {
        /* Grow the cache if necessary. */
        if (size == cap)
        {
            size_t newcap = (cap ? cap << 1 : 16);
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return 0;   /* allocation failure → undefined result */
            cache = tmp;
            cap   = newcap;
        }

        /* Generate terms until the nth is computed. */
        while (size < n)
        {
            seq_t k = next_k++;
            if (has_adjacent_45_in_base7(k) && !has_adjacent_45_in_base7(k + 1))
            {
                cache[size++] = k;
            }
        }
    }
    return cache[n - 1];
}
