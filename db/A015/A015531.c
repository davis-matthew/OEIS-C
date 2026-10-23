/*
 * A015531  Linear 2nd order recurrence: a(n) = 4*a(n-1) + 5*a(n-2).
 * https://oeis.org/A015531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 5*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size of cache   */
    static size_t size  = 0;   /* number of elements stored */

    if (cache == NULL) {
        /* initial allocation for first two terms */
        cap = 8;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        size = 2;
    }

    /* extend cache if necessary */
    if ((size_t)n >= size) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cap = new_cap;
        /* compute missing terms */
        for (size_t i = size; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)4 * cache[i - 1] + (seq_t)5 * cache[i - 2];
        }
        size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
