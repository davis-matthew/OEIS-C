/*
 * A122174  First row sum of the matrix M^n, where M is the 5 X 5 matrix
 *          {{0,-1,-1,-1,-1}, {-1,0,-1,-1,0}, {-1,-1,0,0,0}, {-1,-1,0,1,0},
 *          {-1,0,0,0,1}}.
 * https://oeis.org/A122174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = 2*a(n-1)+5*a(n-2)-13*a(n-3)+7*a(n-4)-a(n-5); Initial
 *            values: a(0)=1, a(1)=-4, a(2)=6, a(3)=-24, a(4)=41
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 23, i64 n <= 47, i128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of computed values, grows as needed */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of elements cached */

    /* Initialise cache with the first five terms */
    if (cache_size == 0) {
        cache_size = 5;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-4;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)-24;
        cache[4] = (seq_t)41;
    }

    /* If the requested index is within the cache, return it directly */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache to accommodate n+1 elements */
    size_t new_size = (size_t)(n + 1);
    if (new_size > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) /* Handle allocation failure */
            return cache[cache_size - 1]; /* fallback */
        cache = new_cache;

        /* Compute new entries using the recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = 2 * cache[i - 1] + 5 * cache[i - 2]
                       - 13 * cache[i - 3] + 7 * cache[i - 4]
                       - cache[i - 5];
        }
        cache_size = new_size;
    }

    return cache[n];
}
