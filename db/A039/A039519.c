/*
 * A039519  Numbers whose base-12 representation has the same nonzero number
 *          of 2's and 8's.
 * https://oeis.org/A039519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   none (search algorithm)
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_equal_nonzero_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches natural numbers in increasing order until the n‑th
   term of OEIS sequence A039519 is found.  A term is a number whose
   base‑12 representation contains the same non‑zero number of 2's and 8's. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_nonzero_counts.h"

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_sz = 0;
    static idx_t last_checked = 0;

    /* Build up the cache until it contains at least n terms */
    while (cache_sz < n)
    {
        /* Check the next integer */
        ++last_checked;
        if (has_equal_nonzero_counts(last_checked))
        {
            if (cache_sz == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();         /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = last_checked;
        }
    }

    return cache[n - 1];
}
