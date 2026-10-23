/*
 * A126940  "Model 2" for number of free alkanes on n points.
 * https://oeis.org/A126940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A126940
    Number of free alkanes on n points.
    Recurrence:
        a(0) = 1,  a(1) = 1,  a(2) = 3,  a(3) = 11,  a(4) = 35,  a(5) = 109
        a(n) = 2*a(n-1) + a(n-2) + 7*a(n-3) + 4*a(n-4) + 4*a(n-5) + a(n-6)   for n ≥ 6
*/

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;        /* size of the allocated cache */
    static idx_t last_computed = 0;     /* highest index already computed */

    /* initial allocation and base terms */
    if (cache == NULL) {
        /* start with room for at least 8 values */
        cache_size = 8;
        cache = (seq_t *)realloc(NULL, cache_size * sizeof(seq_t));
        if (cache == NULL) return 0;   /* out of memory – undefined behaviour */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 11;
        cache[4] = 35;
        cache[5] = 109;
        last_computed = 5;
    }

    /* enlarge the cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        while (new_size <= n) new_size <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* out of memory – undefined behaviour */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            cache[i] = 2 * cache[i - 1]
                     + cache[i - 2]
                     + 7 * cache[i - 3]
                     + 4 * cache[i - 4]
                     + 4 * cache[i - 5]
                     + cache[i - 6];
        }
        last_computed = n;
    }

    return cache[n];
}
