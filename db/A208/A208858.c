/*
 * A208858  Number of n X 2 0..3 arrays with new values 0..3 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or left-upward diagonal neighbors.
 * https://oeis.org/A208858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*a(n-1) - 48*a(n-2) + 36*a(n-3) Initial values: a(1)=2,
 *            a(2)=11, a(3)=127, a(4)=1691
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for the OEIS sequence A208858 */
seq_t a(idx_t n)
{
    /* Cache of previously computed terms.
       index 0 is unused; we keep it to simplify sizing. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialize cache with the first four terms when first called. */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * (cache_size + 1));
        if (!cache) exit(1);               /* memory allocation failed */
        cache[0] = 0;                       /* unused */
        cache[1] = 2;
        cache[2] = 11;
        cache[3] = 127;
        cache[4] = 1691;
    }

    /* Expand cache and compute missing terms up to n. */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (new_size + 1));
        if (!new_cache) {
            free(cache);
            exit(1);
        }
        cache = new_cache;
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            cache[i] = (seq_t)17 * cache[i-1] - (seq_t)48 * cache[i-2] + (seq_t)36 * cache[i-3];
        }
        cache_size = n;
    }

    return cache[n];
}
