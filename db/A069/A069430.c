/*
 * A069430  Half the number of 4 X n binary arrays with no path of adjacent
 *          1's or adjacent 0's from top row to bottom row.
 * https://oeis.org/A069430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19*a(n-1) - 98*a(n-2) + 180*a(n-3) - 80*a(n-4) -
 *            16*a(n-5) (n>=6) Initial values: a(1)=7, a(2)=88, a(3)=1074,
 *            a(4)=13002, a(5)=157058
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values, indexed by n (1‑based) */
    static seq_t *cache      = NULL;
    static idx_t  cache_max  = 0;     /* maximum n computed so far */
    static size_t cache_cap  = 0;     /* allocated capacity */

    /* Allocate or extend the cache as needed */
    if (cache == NULL) {
        cache_cap = 64;                      /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)0;                 /* unused slot 0 */
    }

    if ((size_t)n >= cache_cap) {
        /* Grow capacity to at least n+1 */
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n if necessary */
    if (n > cache_max) {
        static const seq_t init[6] = {0, 7, 88, 1074, 13002, 157058};

        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i <= 5) {
                cache[i] = init[i];
            } else {
                seq_t v = 19 * cache[i - 1]
                        - 98 * cache[i - 2]
                        + 180 * cache[i - 3]
                        - 80 * cache[i - 4]
                        - 16 * cache[i - 5];
                cache[i] = v;
            }
        }
        cache_max = n;
    }

    return cache[n];
}
