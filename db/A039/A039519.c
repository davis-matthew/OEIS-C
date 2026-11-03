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
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches natural numbers in increasing order until the n‑th
   term of OEIS sequence A039519 is found.  A term is a number whose
   base‑12 representation contains the same non‑zero number of 2's and 8's. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether a base‑12 number has the same non‑zero number of 2's and 8's. */
static bool has_equal_nonzero_counts(seq_t x)
{
    unsigned int c2 = 0, c8 = 0;
    while (x > (seq_t)0)
    {
        seq_t d = x % (seq_t)12;
        if (d == (seq_t)2) ++c2;
        else if (d == (seq_t)8) ++c8;
        x /= (seq_t)12;
    }
    return c2 == c8 && c2 > 0;
}

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
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();         /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = last_checked;
        }
    }

    return cache[n - 1];
}
