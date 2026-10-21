/*
 * A204091  The number of 1 X n Haunted Mirror Maze puzzles with a unique
 *          solution ending with a mirror.
 * https://oeis.org/A204091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 2*a(n-2) a(0)=1, a(1)=2, a(2)=10
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array of computed terms */
    static idx_t cache_cap = 0;   /* allocated number of entries */
    static idx_t cache_len = 0;   /* number of terms stored (last index+1) */

    /* initialise cache with first three terms if necessary */
    if (cache == NULL) {
        cache_cap = 3;
        cache_len = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;  /* a(0) */
        /* a(1) and a(2) are needed for the recurrence */
        cache[1] = (seq_t)2;  /* a(1) */
        cache[2] = (seq_t)10; /* a(2) */
    }

    /* expand cache if requested index is beyond current capacity */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute all missing terms up to n using the recurrence */
    if (cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence: a(i) = 5*a(i-1) - 2*a(i-2) */
            cache[i] = (seq_t)5 * cache[i-1] - (seq_t)2 * cache[i-2];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
