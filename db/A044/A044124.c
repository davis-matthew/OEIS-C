/*
 * A044124  Numbers k such that string 3,4 occurs in the base 6 representation
 *          of k but not of k-1.
 * https://oeis.org/A044124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 2034, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 8551 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044124
 * Numbers k such that the string "34" occurs in the base‑6 representation
 * of k but not of k-1.  The function searches for the next qualifying
 * integer and memoises results.  No simple closed formula is known,
 * so a straightforward search suffices within the required limits.
 */

#include <stdbool.h>
#include <stdlib.h>   /* for realloc and abort */
#include "../utils/mem.h"

/* Return true iff the base‑6 representation of x contains the
   consecutive digits 3,4. */
static bool has34_base6(seq_t x)
{
    /* Get the least significant digit first. */
    seq_t prev = x % 6;
    x = x / 6;

    while (x > 0)
    {
        seq_t curr = x % 6;
        if (curr == 3 && prev == 4)
            return true;
        prev = curr;
        x = x / 6;
    }
    return false;
}

/* Return the n-th term of the sequence.  n is 1‑based. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* array of computed terms */
    static size_t cache_len = 0;    /* number of terms stored */
    static size_t cache_cap = 0;    /* allocated capacity */
    static seq_t next_search = 1;   /* smallest candidate not yet examined */

    /* Grow the cache until we have at least n terms. */
    while (cache_len < n)
    {
        seq_t k = next_search;

        /* Find the next k satisfying the condition. */
        while (true)
        {
            if (has34_base6(k) && !has34_base6(k - 1))
            {
                /* Ensure capacity */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp)
                        abort();   /* memory exhaustion */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
                next_search = k + 1;
                break;
            }
            k = k + 1;   /* test next candidate */
        }
    }

    return cache[n - 1];
}
