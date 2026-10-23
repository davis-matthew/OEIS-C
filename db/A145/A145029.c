/*
 * A145029  Pentanacci numbers: a(n)=a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5),
 *          {1,2,3,4,5...}.
 * https://oeis.org/A145029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5) for n>5 with initial
 *            values a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/*  seq_t and idx_t are provided by the build environment.  */
extern idx_t _floor_isqrt_u64(idx_t); /* unused, keeps the header */

seq_t a(idx_t n)
{
    /* Handle the first five terms immediately. */
    if (n <= 5) {
        static const seq_t init[5] = { (seq_t)1, (seq_t)2, (seq_t)3,
                                       (seq_t)4, (seq_t)5 };
        return init[n - 1];
    }

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity (in elements) */
    static size_t cache_len = 0;   /* number of terms that have been computed */

    /* Initialise cache with the first five terms if we haven't yet. */
    if (cache == NULL) {
        cache_cap = (size_t)n > 5 ? (size_t)n : 5;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) { /* out of memory – let the program terminate */ }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)5;
        cache_len = 5;
    }

    /* Grow the cache if necessary. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* out of memory – let the program terminate */ }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        /* a(i+1) = a(i) + a(i-1) + a(i-2) + a(i-3) + a(i-4) */
        cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3]
                 + cache[i - 4] + cache[i - 5];
    }
    cache_len = (size_t)n;

    return cache[n - 1];
}
