/*
 * A044006  Numbers k such that 1 and 3 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1262, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Computation: numbers k such that the decimal representation of k
   contains the substring "13" or "31" but the decimal representation
   of k+1 does not.  The function searches forwards and memoises all
   found terms, so that calls are fast even when indices are not in
   increasing order. */

static inline bool has13or31(idx_t x)
{
    int prev = -1;
    while (x > 0)
    {
        int cur = x % 10;
        if ((cur == 1 && prev == 3) || (cur == 3 && prev == 1))
            return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* memoised cache of found terms */
    static seq_t  *cache   = NULL;          /* array of seq_t values   */
    static size_t cache_cap = 0;            /* current capacity        */
    static size_t cache_len = 0;            /* number of terms stored  */
    static idx_t  next_candidate = 1;       /* next integer to test    */

    /* initialise cache on first call */
    if (cache == NULL)
    {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have the requested index */
    while (cache_len < n)
    {
        if (cache_len == cache_cap)               /* grow the array */
        {
            cache_cap <<= 1;                      /* multiply by 2 */
            cache = (seq_t *)realloc(cache,
                                      cache_cap * sizeof(seq_t));
        }

        /* test the current candidate */
        if (has13or31(next_candidate) &&
            !has13or31(next_candidate + 1))
        {
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];   /* n is 1‑based */
}
