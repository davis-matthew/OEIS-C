/*
 * A188708  Number of 4 X n binary arrays without the pattern 0 0 diagonally
 *          or vertically.
 * https://oeis.org/A188708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 12*a(n-2) + 4*a(n-3), a(1)=8, a(2)=49,
 *            a(3)=304
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache[0] unused; cache[1]..cache[cache_len] store values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* largest index computed so far */

    /* Initialise cache on first call */
    if (cache == NULL) {
        /* Allocate space for indices 0..3 (index 0 unused) */
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failed */
        cache[0] = (seq_t)0;            /* unused */
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)49;
        cache[3] = (seq_t)304;
        cache_len = 3;
    }

    /* Return cached value if already computed */
    if (n <= cache_len) {
        return cache[n];
    }

    /* Resize cache to hold up to index n */
    size_t new_capacity = (size_t)n + 1;   /* need indices 0..n inclusive */
    seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
    if (!tmp) return (seq_t)0;             /* allocation failed */
    cache = tmp;

    /* Compute missing terms iteratively using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        cache[i] = (seq_t)8  * cache[i - 1]
                 - (seq_t)12 * cache[i - 2]
                 + (seq_t)4  * cache[i - 3];
    }

    cache_len = n;
    return cache[n];
}
