/*
 * A021772  Expansion of 1/((1-x)(1-4x)(1-5x)(1-9x)).
 * https://oeis.org/A021772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   recurrence of order 4 * a(0) = 1, a(1) = 19, a(2) = 242, a(3) =
 *            2618 * a(n) = 19*a(n-1) - 119*a(n-2) + 281*a(n-3) - 180*a(n-4)
 *            for n >= 4 * * This function memoises previously computed terms
 *            in a static cache * so that calls with arbitrary n are answered
 *            in amortised O(n) time. * It works for seq...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>   /* for abort() */

seq_t a(idx_t n)
{
    /* static cache: array of computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;       /* capacity of cache[] */
    static idx_t filled = 0;     /* number of terms already initialized */

    /* initialise the cache with the first four terms only once */
    if (!cache) {
        /* allocate four slots; cast required for C++ */
        cache = (seq_t*)calloc(4, sizeof(seq_t));
        if (!cache) abort();     /* memory exhausted */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)19;
        cache[2] = (seq_t)242;
        cache[3] = (seq_t)2618;
        cap = 4;
        filled = 4;
    }

    /* if the requested term is already in the cache, return it */
    if (n < filled)
        return cache[n];

    /* grow the cache if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();       /* memory exhausted */
        cache = tmp;
        cap   = newcap;
    }

    /* compute the missing terms using the recurrence */
    for (idx_t i = filled; i <= n; ++i) {
        seq_t t  = 19 * cache[i - 1];
        t        -= 119 * cache[i - 2];
        t        += 281 * cache[i - 3];
        t        -= 180 * cache[i - 4];
        cache[i] = t;
    }
    filled = n + 1;

    return cache[n];
}
