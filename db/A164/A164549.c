/*
 * A164549  a(n) = 4*a(n-1) + 2*a(n-2) for n > 1; a(0) = 1, a(1) = 6.
 * https://oeis.org/A164549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2), a(0)=1, a(1)=6
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 1000 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void seq_free_array(seq_t* p)          { mem_free(p); }

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialize base case if necessary */
    if (cache_size == 0) {
        cache_size = 2;
        cache = seq_alloc_array(cache_size);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
    }

    /* Return known value if already computed */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache to accommodate n */
    seq_t *new_cache = seq_alloc_array(n + 1);
    for (idx_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];
    seq_free_array(cache);
    cache = new_cache;
    cache_size = n + 1;

    /* Compute missing values */
    for (idx_t i = 2; i <= n; ++i) {
        cache[i] = (seq_t)4 * cache[i - 1] + (seq_t)2 * cache[i - 2];
    }

    return cache[n];
}
