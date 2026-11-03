/*
 * A224136  Number of 5 X n 0..1 arrays with rows nondecreasing and
 *          antidiagonals unimodal.
 * https://oeis.org/A224136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) for n > 9, with a(1)=32, a(2)=243, a(3)=748, a(4)=1635,
 *            a(5)=3149, a(6)=5648, a(7)=9577, a(8)=15505, a(9)=24141
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 124, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Validated: standard, pass, 48 terms through n = 207 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t compute_step(idx_t k, const seq_t *cache)
{
    seq_t res = cache[k-2] * (seq_t)6;   /* a(k-1) */
    res -= cache[k-3] * (seq_t)15;      /* a(k-2) */
    res += cache[k-4] * (seq_t)20;      /* a(k-3) */
    res -= cache[k-5] * (seq_t)15;      /* a(k-4) */
    res += cache[k-6] * (seq_t)6;       /* a(k-5) */
    res -= cache[k-7];                  /* a(k-6) */
    return res;
}

seq_t a(idx_t n)
{
    /* Base values (indices 1..9).  Use a static array for ease of use. */
    static const seq_t base[9] = {
        32, 243, 748, 1635, 3149, 5648, 9577, 15505, 24141
    };

    /* Cache of previously computed terms, for memoisation. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored (>=0) */

    /* First call: initialise the cache with the nine known values. */
    if (cache_len == 0) {
        cache_len = 9;
        cache = mem_new(seq_t, (size_t)cache_len);
        if (!cache)
            return 0; /* allocation failure */
        for (size_t i = 0; i < 9; ++i)
            cache[i] = base[i];
    }

    /* If the requested term is already calculated, return it. */
    if (n <= cache_len)
        return cache[n-1];

    /* Extend the cache to hold terms up to n. */
    size_t new_len = (size_t)n;
    seq_t *new_cache = mem_resize(seq_t, cache, new_len);
    if (!new_cache) {
        /* Allocation failed: free old cache and return 0. */
        mem_free(cache);
        cache = NULL;
        cache_len = 0;
        return 0;
    }
    cache = new_cache;

    /* Compute missing terms using the recurrence relation. */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        cache[k-1] = compute_step(k, cache);
    }
    cache_len = new_len;

    return cache[n-1];
}
