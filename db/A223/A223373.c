/*
 * A223373  3 X 3 square grid graph coloring a rectangular array: number of n
 *          X 2 0..8 arrays where 0..8 label nodes of the square grid graph
 *          and every array movement to a horizontal or vertical neighbor
 *          moves along an edge of this graph.
 * https://oeis.org/A223373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 24, a(2) = 144 a(n) = 7*a(n-1) - 4*a(n-2) for n ≥ 3 (OEIS
 *            A223373)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    /* ensure n fits in size_t for indexing */
    size_t idx = (size_t)n;

    /* allocate or grow the cache if needed */
    if (cache == NULL) {
        cache_cap = (idx < 2 ? 2 : idx);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
    } else if (idx > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap < idx) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* fill cache up to idx */
    while (cache_size < idx) {
        if (cache_size == 0) {
            cache[cache_size] = (seq_t)24;
        } else if (cache_size == 1) {
            cache[cache_size] = (seq_t)144;
        } else {
            seq_t val = mul_small(cache[cache_size - 1], (unsigned int)7)
                        - mul_small(cache[cache_size - 2], (unsigned int)4);
            cache[cache_size] = val;
        }
        cache_size++;
    }

    return cache[idx - 1];
}
