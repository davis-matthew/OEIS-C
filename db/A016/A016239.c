/*
 * A016239  Expansion of 1/((1-x)*(1-5*x)*(1-12*x)).
 * https://oeis.org/A016239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) - 60*a(n-2) + 1, a(0) = 1, a(1) = 18
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Expand cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = seq_realloc(cache, cache_size, new_size);
        if (!new_cache) return 0; /* allocation failure – return 0 as fallback */

        /* Initialise new entries */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i == 0) {
                new_cache[0] = (seq_t)1;
            } else if (i == 1) {
                new_cache[1] = (seq_t)18;
            } else {
                /* a(i) = 17*a(i-1) - 60*a(i-2) + 1 */
                new_cache[i] = (seq_t)(17 * new_cache[i - 1] - 60 * new_cache[i - 2] + 1);
            }
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
