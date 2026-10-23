/*
 * A190097  Number of rhombuses on a (n+1)X9 grid.
 * https://oeis.org/A190097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of rhombuses on a (n+1)×9 grid. For n ≤ 51 the
 *            value is obtained by a direct enumeration of all rhombuses. For
 *            n > 51 the known linear recurrence a(n) = 2*a(n-1) - 2*a(n-3) +
 *            a(n-4) is used. In both cases the sequence is stored in a static
 *            cache so that calls in any order are efficient
 * Range:     u8 n <= 2, u16 n <= 53, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   _count_rhombuses
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_count_rhombuses.h"

/* Main routine: returns a(n). */
seq_t a(idx_t n)
{
    /* dynamically grow cache up to maximum n requested */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t highest_computed = 0;

    if (n + 1 > cache_size) {
        size_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (cache) {
            for (size_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache      = new_cache;
        cache_size = new_size;
    }

    for (idx_t i = highest_computed + 1; i <= n; ++i) {
        if (i <= 51) {
            cache[i] = _count_rhombuses(i);
        } else {
            cache[i] = 2 * cache[i - 1]
                     - 2 * cache[i - 3]
                     + cache[i - 4];
        }
    }
    if (n > highest_computed)
        highest_computed = n;

    return cache[n];
}
