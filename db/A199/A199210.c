/*
 * A199210  a(n) = (11*4^n + 1)/3.
 * https://oeis.org/A199210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*4^n + 1)/3 Recurrence: a(0) = 4, a(n) = 4*a(n-1) - 1
 *            (n > 0)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Static cache: cache[i] = a(i) for i < cache_size */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_size = 0;  /* number of elements currently stored (>=1 if cache != NULL) */

    size_t need = (size_t)n + 1;   /* number of entries that must exist */

    /* Allocate or grow the cache as needed */
    if (!cache) {
        cache = seq_alloc_array(need);
        if (!cache) return 0;   /* allocation failure – unlikely in tests */
        cache_cap = need;
        cache_size = 1;
        cache[0] = 4;          /* a(0) = 4 */
    } else if (need > cache_cap) {
        seq_t *new_cache = seq_alloc_array(need);
        if (!new_cache) return 0;
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free_seq_array(cache);
        cache = new_cache;
        cache_cap = need;
        /* cache_size remains unchanged */
    }

    /* Compute missing terms up to n */
    if (n >= cache_size) {
        size_t i;
        for (i = cache_size; i <= (size_t)n; ++i) {
            /* a(i) = 4*a(i-1) - 1 */
            cache[i] = (cache[i - 1] << 2) - 1;
        }
        cache_size = need;
    }

    return cache[(size_t)n];
}
