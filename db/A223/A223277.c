/*
 * A223277  Rolling icosahedron face footprints: number of n X 3 0..19 arrays
 *          starting with 0 where 0..19 label faces of an icosahedron and
 *          every array movement to a horizontal, diagonal or antidiagonal
 *          neighbor moves across an icosahedral edge.
 * https://oeis.org/A223277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 12*a(n-2) with a(1) = 9, a(2) = 87 (offset 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset 1, but guard against 0 */

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Expand cache to hold indices 0..n */
    if (cache_size <= n) {
        idx_t new_size = n + 1;                    /* include index 0 */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                        /* allocation failed */
        /* Initialise new entries to 0 */
        for (idx_t i = cache_size; i <= n; ++i) {
            tmp[i] = 0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Ensure base values are set (safe to assign repeatedly) */
    cache[1] = 9;
    if (n >= 2) cache[2] = 87;

    /* Compute missing terms using the recurrence */
    for (idx_t i = 3; i <= n; ++i) {
        cache[i] = 11 * cache[i - 1] - 12 * cache[i - 2];
    }

    return cache[n];
}
