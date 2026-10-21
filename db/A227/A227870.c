/*
 * A227870  Numbers with equal number of even and odd digits.
 * https://oeis.org/A227870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 3420, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: iteratively search for numbers with an equal number of
   even and odd decimal digits, caching all found terms.  The cache
   allows the function to answer any call for n≥1 in O(1) after the
   first search. */

/* NOTE: This implementation uses only C constructs and does not
   depend on any of the auxiliary utilities listed in the problem
   statement.  It works for any seq_t/idx_t provided by the build
   system. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *cache     = NULL;   /* array of found numbers          */
    static size_t cache_sz  = 0;      /* number of terms currently stored*/
    static size_t cache_cap = 0;      /* allocated capacity               */

    /* Last integer tested (inclusive).  We start after 9 because the
       first term is 10. */
    static idx_t last_candidate = 9;

    /* If the requested term is already in the cache, return it. */
    if (n <= cache_sz) {
        return cache[(size_t)(n - 1)];
    }

    /* Extend the cache until it contains at least n terms. */
    while (cache_sz < (size_t)n) {
        /* Advance to the next integer candidate. */
        last_candidate++;

        /* Count even and odd digits of last_candidate. */
        idx_t x          = last_candidate;
        size_t even_cnt = 0, odd_cnt = 0;

        if (x == 0) {
            /* Number 0 (only occurs for the first test if we ever
               evaluate it); it has one even digit. */
            even_cnt = 1;
        } else {
            while (x > 0) {
                unsigned d = (unsigned)(x % 10);
                if ((d & 1) == 0)
                    ++even_cnt;
                else
                    ++odd_cnt;
                x /= 10;
            }
        }

        /* Store the number if it has an equal number of even and odd
           digits. */
        if (even_cnt == odd_cnt) {
            if (cache_sz == cache_cap) {
                /* Grow the cache: double its capacity or start at 8. */
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) {
                    /* Allocation failed; exit with an error. */
                    abort();
                }
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = (seq_t)last_candidate;
        }
    }

    return cache[(size_t)(n - 1)];
}
