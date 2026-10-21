/*
 * A208884  a(n) = (a(n-1) + n)/2^k where 2^k is the largest power of 2
 *          dividing a(n-1) + n, for n>1 with a(1)=1.
 * https://oeis.org/A208884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A208884  –  a(n) = odd_part(a(n‑1) + n),  a(1) = 1
 *
 *  This implementation caches all previously computed values so that
 *  calls in any order are fast.  It uses standard allocation functions
 *  and the supplied odd_part routine from "../utils/odd_part.h".
 */
#include <stdlib.h>
#include "../utils/odd_part.h"

seq_t a(idx_t n)
{
    /* cache[i] holds a(i) for i >= 1; index 0 is unused */
    static seq_t *cache   = NULL;           /* array of a(i) values */
    static idx_t  cache_up = 0;            /* highest computed index */
    static size_t cache_cap = 0;           /* allocated capacity (elements) */

    if (n == 0)          /* offset starts at 1, return 0 for 0 */
        return 0;

    /* Grow the cache if necessary to reach index n */
    if (n > cache_up) {
        size_t new_cap = (size_t)(n + 1);   /* include index 0 */

        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();              /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Initialise the very first term on the first call */
        if (cache_up == 0) {
            cache[1] = (seq_t)1;
            cache_up = 1;
        }

        /* Compute all missing terms sequentially */
        for (idx_t i = cache_up + 1; i <= n; ++i) {
            seq_t sum = cache[i - 1] + (seq_t)i;
            cache[i] = odd_part(sum);
        }
        cache_up = n;
    }

    return cache[n];
}
