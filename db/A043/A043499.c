/*
 * A043499  Numbers having three 2's in base 10.
 * https://oeis.org/A043499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 698, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer that contains exactly three digits '2' in its decimal
   representation.  Indexing starts at 1, so a(1) = 222.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of found numbers */
    static seq_t *cache = NULL;   /* array of results */
    static size_t  cache_len = 0; /* number of stored results */
    static size_t  cache_cap = 0; /* allocated capacity of cache */
    static seq_t  current = 0;    /* last integer tested (for generation) */

    if (n == 0) { /* offset 1; return 0 for safety */
        return (seq_t)0;
    }

    /* Return cached result if already computed. */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Ensure cache has space for the new item(s). */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate numbers until we have enough. */
    while (cache_len < n) {
        ++current; /* move to next candidate */

        /* Count digits equal to 2. */
        seq_t tmp = current;
        int  cnt = 0;
        while (tmp > (seq_t)0) {
            if ((tmp % (seq_t)10) == (seq_t)2) {
                ++cnt;
                if (cnt > 3) break; /* early exit */
            }
            tmp /= (seq_t)10;
        }

        if (cnt == 3) {
            /* Expand cache if necessary. */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = current;
        }
    }

    return cache[n - 1];
}
