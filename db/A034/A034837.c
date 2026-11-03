/*
 * A034837  Numbers that are divisible by the first, i.e., the leftmost,
 *          digit.
 * https://oeis.org/A034837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, look, easy
 * Formula:   not stated
 * Range:     u8 n <= 169, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9803 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A034837: a(n) is the nth positive integer that is divisible by its most
 * significant (leftmost) decimal digit.  The definition is not given by a simple
 * closed formula, so we generate the terms by successive inspection of
 * integers.  The function memoises results for efficiency. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>   /* for malloc, realloc */
#include "../utils/mem.h"

static seq_t leftmost_digit(seq_t x)
{
    /* Return the leftmost decimal digit of x.  x is always > 0. */
    while (x >= (seq_t)10)
        x /= (seq_t)10;
    return x;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* undefined for n = 0; return 0 */
        return (seq_t)0;

    /* Memoisation cache */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* allocated capacity */
    static seq_t next_check  = (seq_t)0;  /* the last integer examined */

    /* Lazy allocation of the cache array */
    if (cache == NULL)
    {
        cache_cap = 16;   /* initial capacity */
        cache = mem_new(seq_t, cache_cap);
        /* Assume malloc succeeds; in production code check for NULL. */
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        ++next_check;

        seq_t first = leftmost_digit(next_check);
        /* first digit is in [1,9] for positive integers */
        if ((next_check % first) == (seq_t)0)
        {
            /* Grow the cache if necessary */
            if (cache_len >= cache_cap)
            {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = next_check;
        }
    }

    return cache[(size_t)n - (size_t)1];
}
