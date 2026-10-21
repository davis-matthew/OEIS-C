/*
 * A093964  a(n) = Sum_{k=1..n} k*k!*C(n,k).
 * https://oeis.org/A093964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} k*k!*C(n,k) = n*((n+1)/(n-1)*a(n-1) -
 *            a(n-2)), a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) { /* grow cache enough to hold indices up to n */
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – aborting as per constraints */
            abort();
        }
        /* initialise new entries */
        if (new_size > cache_size) {
            /* ensure first two entries exist */
            if (cache_size == 0) {
                new_cache[0] = (seq_t)0;
                if (new_size > 1) new_cache[1] = (seq_t)1;
            } else if (cache_size == 1) {
                new_cache[1] = (seq_t)1;
            }
            /* compute remaining terms up to new_size-1 */
            for (idx_t i = cache_size; i < new_size; ++i) {
                if (i < 2) continue; /* a(0), a(1) already set */
                seq_t num = ((seq_t)(i + 1)) * new_cache[i - 1] - ((seq_t)(i - 1)) * new_cache[i - 2];
                new_cache[i] = ((seq_t)i) * (num / (seq_t)(i - 1));
            }
        }
        cache = new_cache;
        cache_size = new_size;
    }
    return cache[n];
}
