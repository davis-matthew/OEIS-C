/*
 * A097593  Number of increasing runs of even length in all permutations of
 *          [n].
 * https://oeis.org/A097593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A097593 computed by the recurrence * a(0) = 0, a(1) = 0, a(2) =
 *            1, a(3) = 4 * a(n) = (2*n-1)*a(n-1) - (n-2)*(n-1)*a(n-2) -
 *            (n-2)*(n-1)*a(n-3) (n >= 4) * This recurrence yields the exact
 *            integer values for all n >= 0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;      /* array of values */
    static idx_t cache_len = 0;      /* number of values stored (cache_len = k means values [0,k-1] are stored) */

    /* Ensure the cache is large enough to hold up to index n */
    if (cache_len <= n) {
        /* Determine new size: at least n+1, but initialise to at least 4 for base cases */
        idx_t new_len = (n > 3) ? n + 1 : 4;
        /* Allocate or resize cache */
        if (cache == NULL) {
            cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        } else {
            cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        }
        if (cache == NULL) { /* allocation failure – signal by returning 0 (unlikely in test harness) */
            return 0;
        }
        /* Initialise base cases if we are building the cache from scratch or extending beyond previous base */
        if (cache_len == 0) {
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)1;
            cache[3] = (seq_t)4;
            cache_len = 4;
        } else if (cache_len <= 3 && cache_len < new_len) {
            /* We are extending the cache but base indices 0..3 not yet set */
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)1;
            cache[3] = (seq_t)4;
            cache_len = 4;
        }
        /* Compute values from current cache_len up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 4) {
                /* already initialized above */
                continue;
            }
            /* recurrence: a(i) = (2*i-1)*a(i-1) - (i-2)*(i-1)*a(i-2) - (i-2)*(i-1)*a(i-3) */
            seq_t term1 = (seq_t)(2 * i - 1) * cache[i - 1];
            seq_t coeff = (seq_t)(i - 2) * (i - 1);
            seq_t term2 = coeff * cache[i - 2];
            seq_t term3 = coeff * cache[i - 3];
            cache[i] = term1 - term2 - term3;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
