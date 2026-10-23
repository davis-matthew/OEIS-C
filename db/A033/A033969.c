/*
 * A033969  Trajectory of 1 under map n->25n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 21, u64 n <= 56, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1
   For n > 0:  a(n) = a(n-1)/2            if a(n-1) is even
               a(n) = 25*a(n-1) + 1      if a(n-1) is odd */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache      = NULL;   /* array of terms          */
    static size_t cache_len  = 0;      /* number of computed terms*/
    static size_t cache_cap  = 0;      /* capacity of 'cache'     */

    /* initialise first term when called for the first time   */
    if (cache == NULL)
    {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* enlarge cache if the requested index is not yet covered */
    if (n + (idx_t)1 > (idx_t)cache_cap)   /* (n+1) needed because we index 0..n inclusive */
    {
        size_t needed = (size_t)n + 1;
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < needed) new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n inclusive */
    for (; cache_len <= n; ++cache_len)
    {
        seq_t prev = cache[cache_len - 1];
        seq_t next = (prev & (seq_t)1) ?
                      prev * (seq_t)25 + (seq_t)1
                    : prev / (seq_t)2;
        cache[cache_len] = next;
    }

    return cache[(size_t)n];
}
