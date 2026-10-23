/*
 * A121202  a(n) = 12*n + 7^n - 5^n.
 * https://oeis.org/A121202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=14, a(2)=48, a(3)=254;
   a(n)=14*a(n-1)-60*a(n-2)+82*a(n-3)-35*a(n-4) */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* mutable cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of initialized elements */
    static size_t cache_cap  = 0;   /* capacity of the cache buffer */

    /* if value already cached, just return it */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* ensure buffer large enough */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap < needed)
            new_cap <<= 1;
        /* realloc preserves existing contents */
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i == 0)
            cache[i] = 0;
        else if (i == 1)
            cache[i] = 14;
        else if (i == 2)
            cache[i] = 48;
        else if (i == 3)
            cache[i] = 254;
        else
            cache[i] = 14*cache[i-1] - 60*cache[i-2] + 82*cache[i-3] - 35*cache[i-4];
    }
    cache_size = needed;

    return cache[(size_t)n];
}
