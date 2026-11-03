/*
 * A118951  Numbers containing at least one composite digit.
 * https://oeis.org/A118951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = n-th positive integer that contains at least one
   composite digit (4, 6, 8, or 9) in its decimal representation. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether a number contains at least one composite digit.
   Composite digits are 4, 6, 8, and 9. */
static bool has_composite_digit(seq_t x)
{
    if (x == 0U)   /* the number 0 itself has no digits ≥4, so it is not included */
        return false;

    while (x != 0U)
    {
        unsigned d = (unsigned)(x % 10U);
        if (d == 4U || d == 6U || d == 8U || d == 9U)
            return true;
        x /= 10U;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Cache of computed values.  Filled lazily on demand. */
    static seq_t *cache          = NULL;   /* array of terms */
    static size_t cache_cap      = 0;      /* current capacity of cache */
    static size_t cache_len      = 0;      /* number of terms already cached */
    static seq_t next_candidate   = 1U;    /* next integer to test */

    if (n == 0U)           /* no term defined for n=0 – return a safe sentinel */
        return 0U;

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n)
    {
        seq_t x = next_candidate++;

        if (has_composite_digit(x))
        {
            /* Ensure enough capacity. */
            if (cache_len >= cache_cap)
            {
                size_t new_cap = (cache_cap == 0U) ? 4U : cache_cap * 2U;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (tmp == NULL)
                    abort();                 /* unrecoverable allocation failure */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = x;
        }
    }

    return cache[(size_t)n - 1U];
}
