/*
 * A202828  Expansion of e.g.f.: exp(4*x/(1-2*x)) / sqrt(1-4*x^2).
 * https://oeis.org/A202828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   alloc_cache, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*(n+1)*a(n-1) + 4*(n-1)*(n+1)*a(n-2)
     - 8*(n-1)*(n-2)^2*a(n-3)   for n ≥ 3
   Initial values: a(0)=1, a(1)=4, a(2)=36 */
#include <stddef.h>
#include <stdbool.h>

/* Helper to allocate a cache array capable of holding n+1 elements. */
#include "../utils/alloc_cache.h"
#include "../utils/grow_seq.h"

/* Return the n-th term of A202828.  The function is memoized and works
 * correctly for any order of calls.  It uses the recurrence relation
 * above together with the initial values. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  max_computed = (idx_t)-1;   /* highest index whose value
                                                is known (ever set) */

    /* Ensure the cache is large enough for indices 0..n. */
    size_t req_cap = (size_t)n + 1;
    if (req_cap > cache_cap) {
        /* Allocate or grow the cache. */
        if (cache == NULL) {
            /* alloc_cache expects n as the highest index to store,
             * so pass (idx_t)(req_cap-1). */
            cache = alloc_cache((idx_t)(req_cap - 1));
        } else {
            grow_seq((seq_t **)&cache, &cache_cap, req_cap);
        }
        cache_cap = req_cap;
    }

    /* Initialize the base values in the cache (if the capacity allows). */
    cache[0] = (seq_t)1;
    if (cache_cap > 1)
        cache[1] = (seq_t)4;
    if (cache_cap > 2)
        cache[2] = (seq_t)36;

    /* Update max_computed if this is the first call. */
    if (max_computed == (idx_t)-1)
        max_computed = (n < 2) ? n : 2;

    /* Compute any missing terms using the recurrence. */
    idx_t start = max_computed + 1;
    if (start < 3)
        start = 3;                 /* recurrence is valid only for n ≥ 3 */

    for (idx_t i = start; i <= n; ++i) {
        /* Use seq_t for all arithmetic to avoid intermediate overflow. */
        seq_t t1 = (seq_t)2 * (seq_t)(i + 1) * cache[i - 1];
        seq_t t2 = (seq_t)4 * (seq_t)(i - 1) * (seq_t)(i + 1) * cache[i - 2];
        seq_t t3 = (seq_t)8 * (seq_t)(i - 1) * (seq_t)(i - 2) *
                   (seq_t)(i - 2) * cache[i - 3];
        seq_t val = t1 + t2;
        val -= t3;
        cache[i] = val;
    }

    if (n > max_computed)
        max_computed = n;

    return cache[n];
}
