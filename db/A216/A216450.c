/*
 * A216450  a(n) = -10*a(n-1) - 3*a(n-2) + a(n-3) with a(0) = 3, a(1) = -10,
 *          and a(2) = 94.
 * https://oeis.org/A216450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -10*a(n-1) - 3*a(n-2) + a(n-3) initial values: a(0)=3,
 *            a(1)=-10, a(2)=94
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;   /* array of values */
    static idx_t cached_up_to = 0;/* largest index computed */
    static size_t cache_size = 0; /* allocated size */

    /* initialize cache with the first three terms */
    if (cache == NULL) {
        cache = (seq_t*)malloc(3 * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failure unlikely */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)-10;
        cache[2] = (seq_t)94;
        cached_up_to = 2;
        cache_size   = 3;
    }

    /* if the requested index is already cached, return it */
    if (n <= cached_up_to) return cache[n];

    /* grow the cache array if needed */
    size_t needed = (size_t)n + 1;
    if (needed > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failure unlikely */
        cache = tmp;
        cache_size = needed;
    }

    /* compute missing values using the recurrence relation */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        cache[i] = (seq_t)-10 * cache[i - 1]
                 + (seq_t)-3  * cache[i - 2]
                 +           cache[i - 3];
    }
    cached_up_to = n;
    return cache[n];
}
