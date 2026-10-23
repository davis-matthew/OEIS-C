/*
 * A164589  a(n) = ((4 + 3*sqrt(2))*(1 + 2*sqrt(2))^n + (4 - 3*sqrt(2))*(1 -
 *          2*sqrt(2))^n)/8.
 * https://oeis.org/A164589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + 7*a(n-2)   (for n > 1)
   Initial values: a(0) = 1, a(1) = 4
   This uses memoization to support arbitrary call order. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: a[i] = sequence value for index i */
    static seq_t *cache = NULL;
    static idx_t  cache_cap = 0;   /* allocated size (in elements)   */
    static idx_t  cache_len = 0;   /* number of initialized elements */

    /* make sure the table is large enough */
    if (n >= cache_cap)
    {
        idx_t new_cap = cache_cap ? (cache_cap << 1) : 4;
        if (new_cap <= n)          /* ensure we can store a[n]       */
            new_cap = n + 1;

        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialise first two terms if necessary */
    if (cache_len == 0)
    {
        cache[0] = 1;
        cache_len = 1;
    }
    if (cache_len == 1)
    {
        cache[1] = 4;
        cache_len = 2;
    }

    /* compute missing terms up to n */
    while (cache_len <= n)
    {
        seq_t next = 2 * cache[cache_len - 1] + 7 * cache[cache_len - 2];
        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[n];
}
