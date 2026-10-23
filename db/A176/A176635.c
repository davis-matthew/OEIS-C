/*
 * A176635  a(n) = 6*a(n-1)-8*a(n-2) for n > 1; a(0) = 57, a(1) = 242.
 * https://oeis.org/A176635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=57, a(1)=242, a(n)=6*a(n-1)-8*a(n-2) for n>1
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

/* Return the nth term of OEIS sequence A176635. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;          /* allocated capacity */
    static idx_t    max_c = 0;          /* highest index computed */

    /* Lazy initialisation */
    if (cache == NULL) {
        cap   = 8;                                 /* small initial size */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)                      /* allocation failed */
            return (seq_t)0;
        cache[0] = 57;
        cache[1] = 242;
        max_c   = 1;
    }

    /* Grow the array if the requested index is beyond the capacity */
    if ((size_t)n >= cap) {
        size_t new_cap = cap << 1;                /* double */
        if (new_cap <= (size_t)n)
            new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (new_cache == NULL)                   /* allocation failed */
            return (seq_t)0;
        for (size_t i = 0; i < cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* Extend the sequence up to n using the recurrence */
    if (n > max_c) {
        for (idx_t i = max_c + 1; i <= n; ++i) {
            cache[i] = 6 * cache[i - 1] - 8 * cache[i - 2];
        }
        max_c = n;
    }

    return cache[n];
}
