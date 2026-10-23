/*
 * A029162  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^8)*(1-x^10)).
 * https://oeis.org/A029162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of partitions of n into parts 2, 3, 8 and 10 (generating
 *            function 1/((1-x^2)(1-x^3)(1-x^8)(1-x^10)))
 * Range:     u8 n <= 77, u16 n <= 561, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache that grows as needed.  Cache contains
       the number of partitions for every index up to the
       current capacity. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;  /* one past the highest cached index */

    /* Ensure we have computed all values up to n. */
    if ((size_t)n + 1 > cache_cap) {
        /* Grow to at least double the needed size to reduce reallocations. */
        size_t new_cap;
        if (cache_cap == 0)
            new_cap = 1;
        else
            new_cap = cache_cap;
        while (new_cap <= (size_t)n) {
            new_cap *= 2;
        }

        /* Reallocate cache and recompute from scratch. */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache)               /* out of memory – return zero */
            return (seq_t)0;

        /* Initial value: one way to partition zero. */
        cache[0] = (seq_t)1;

        /* Zero remaining entries. */
        for (size_t i = 1; i < new_cap; ++i) {
            cache[i] = (seq_t)0;
        }

        /* Apply the partition DP: for each part, add contributions. */
        const idx_t parts[] = {2, 3, 8, 10};
        const size_t n_parts = sizeof(parts) / sizeof(parts[0]);

        for (size_t p_i = 0; p_i < n_parts; ++p_i) {
            size_t p = (size_t)parts[p_i];
            for (size_t i = p; i < new_cap; ++i) {
                cache[i] += cache[i - p];
            }
        }

        cache_cap = new_cap;
    }

    return cache[(size_t)n];
}
