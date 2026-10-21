/*
 * A213445  Squares containing a digit 2.
 * https://oeis.org/A213445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   has_digit2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the n‑th square that contains the decimal digit 2.
   It searches successive integers, squares them, and keeps the values that
   contain a 2 in a static cache.  Random‑order calls are fast because the
   cache is retained between calls. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_digit2.h"

seq_t a(idx_t n)
{
    /* static cache for already-found terms */
    static seq_t *cache = NULL;
    static size_t sz   = 0;   /* number of stored terms          */
    static size_t cap  = 0;   /* current capacity of cache       */
    static idx_t  next = 1;   /* next integer to test the square */

    /* Grow the cache until we have at least n terms. */
    while (sz < n)
    {
        seq_t sq = (seq_t)next * (seq_t)next;   /* compute i^2 in seq_t */
        if (has_digit2(sq))
        {
            if (sz == cap)   /* need more room */
            {
                size_t newcap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                cache = tmp;
                cap = newcap;
            }
            cache[sz++] = sq;
        }
        ++next;
    }

    return cache[n - 1];
}
