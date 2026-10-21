/*
 * A223330  Rolling cube footprints: number of nX7 0..7 arrays starting with 0
 *          where 0..7 label vertices of a cube and every array movement to a
 *          horizontal or antidiagonal neighbor moves along a corresponding
 *          cube edge.
 * https://oeis.org/A223330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 692*a(n-1) -43569*a(n-2) +847042*a(n-3) -6303164*a(n-4)
 *            +15731352*a(n-5); Initial values: a(1)=729, a(2)=453789,
 *            a(3)=283231809, a(4)=176834343105, a(5)=110409049892673
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* If we already have the requested value, return it. */
    if (cache != NULL && n <= cache_size)
        return cache[n-1];

    /* Initialise the cache with the first five terms if not yet done. */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL)
            return 0; /* allocation failure – undefined but harmless */

        cache[0] = (seq_t)729;
        cache[1] = (seq_t)453789;
        cache[2] = (seq_t)283231809;
        cache[3] = (seq_t)176834343105;
        cache[4] = (seq_t)110409049892673;
    }

    /* Grow the cache and compute any missing terms up to n. */
    if (n > cache_size) {
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (cache == NULL)
            return 0; /* allocation failure – undefined but harmless */

        for (idx_t i = cache_size; i < n; ++i) {
            cache[i] = 692 * cache[i-1]
                       - 43569 * cache[i-2]
                       + 847042 * cache[i-3]
                       - 6303164 * cache[i-4]
                       + 15731352 * cache[i-5];
        }
        cache_size = n;
    }

    return cache[n-1];
}
