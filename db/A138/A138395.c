/*
 * A138395  a(n) = 6*a(n-1) - 3*a(n-2), a(1) = 1, a(2) = 6.
 * https://oeis.org/A138395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 3*a(n-2), a(1)=1, a(2)=6
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   seq_alloc, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1; undefined for 0 */

    /* static cache: a[0] corresponds to a(1) */
    static seq_t *cache = NULL;
    static idx_t cache_n = 0;  /* number of terms stored in cache */

    /* grow the cache if needed */
    if (n > cache_n) {
        /* reallocate to hold at least n terms */
        seq_t *new_cache = seq_realloc(cache, cache_n, n);
        if (!new_cache) return 0; /* allocation failure; caller can handle */
        cache = new_cache;

        /* initialize new entries */
        if (cache_n == 0) {
            cache[0] = (seq_t)1;          /* a(1) */
            if (n >= 2) cache[1] = (seq_t)6;  /* a(2) */
            cache_n = 2;
        } else if (cache_n == 1) {
            cache[1] = (seq_t)6;          /* a(2) */
            cache_n = 2;
        }

        /* compute remaining terms up to n */
        for (idx_t i = cache_n; i < n; ++i) {
            seq_t term_prev = cache[i-1];            /* a(i) where i>=2 */
            seq_t term_prevprev = cache[i-2];        /* a(i-1) where i>=3 */
            cache[i] = (seq_t)6 * term_prev - (seq_t)3 * term_prevprev;
        }
        cache_n = n;
    }

    return cache[n-1];
}
