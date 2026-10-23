/*
 * A145503  a(n+1) = a(n)^2 + 2*a(n) - 2 and a(1) = 3.
 * https://oeis.org/A145503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 3, a(n+1) = a(n)^2 + 2*a(n) - 2 (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms */
    static idx_t cache_len = 0;   /* number of cached terms (a(1) is at cache[0]) */

    if (n == 0) return 0;         /* offset 1 – under‑range value, unused */

    /* Return cached value if available */
    if (n <= cache_len)
        return cache[n - 1];

    /* Allocate a new array big enough for n terms */
    seq_t *new_cache = seq_alloc_array((size_t)n);

    /* Copy any previously cached values */
    if (cache_len > 0) {
        for (size_t i = 0; i < (size_t)cache_len; ++i)
            new_cache[i] = cache[i];
    }

    /* Compute any missing terms, starting from the first missing index */
    idx_t start = cache_len; /* 0‑based index of first missing value */

    if (start == 0) {        /* base case a(1) = 3 */
        new_cache[0] = 3;
        ++start;
    }

    for (idx_t i = start; i < n; ++i) {
        seq_t prev = new_cache[i - 1];
        new_cache[i] = prev * prev + 2 * prev - 2;
    }

    /* Replace old cache with the new one */
    seq_free_array(cache);
    cache     = new_cache;
    cache_len = n;

    return cache[n - 1];
}
