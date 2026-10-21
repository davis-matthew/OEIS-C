/*
 * A207044  Number of (n+1) X 3 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly one clockwise edge increases.
 * https://oeis.org/A207044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = 2*a(n-2) + 5*a(n-3) - a(n-4) - a(n-5) for n>7
   Initial values: a(1)=150, a(2)=270, a(3)=552, a(4)=1182, a(5)=2244,
   a(6)=4722, a(7)=9582 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

/* Compute A207044: number of (n+1)×3 arrays with the edge‑increase property */
seq_t a(idx_t n)
{
    /* Cache of already computed terms (1‑based indexing, cache[0] = a(1)) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of terms stored */

    /* Initial values for n = 1 .. 7 */
    static const seq_t init[7] = {
        150, 270, 552, 1182, 2244, 4722, 9582
    };

    if (n == 0)                /* safety – OEIS offset is 1 */
        return 0;

    /* First call: allocate cache and fill initial values */
    if (cache_size == 0) {
        cache_size = 7;
        cache = alloc_seq((size_t)cache_size);
        if (!cache) return 0;           /* allocation failed */
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = init[i];
    }

    /* Return already known term */
    if (n <= cache_size)
        return cache[n - 1];

    /* Grow cache to hold up to n */
    idx_t new_size = n;
    seq_t *new_cache = alloc_seq((size_t)new_size);
    if (!new_cache) return 0;           /* allocation failed */

    /* Copy existing terms */
    for (idx_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    /* Free old storage */
    free_seq(cache);
    cache = new_cache;

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* a(i) = 2*a(i-2) + 5*a(i-3) - a(i-4) - a(i-5) */
        cache[i - 1] = 2 * cache[i - 3] + 5 * cache[i - 4]
                       - cache[i - 5] - cache[i - 6];
    }

    cache_size = new_size;
    return cache[n - 1];
}
