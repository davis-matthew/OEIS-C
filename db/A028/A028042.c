/*
 * A028042  Expansion of 1/((1-3x)(1-4x)(1-7x)(1-12x)).
 * https://oeis.org/A028042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=26, a(2)=447, a(3)=6484;
   a(n) = 26*a(n-1) - 229*a(n-2) + 816*a(n-3) - 1008*a(n-4)  (n≥4) */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;      /* stores computed terms */
static size_t cache_size = 0;    /* current number of cached terms */

seq_t a(idx_t n)
{
    /* initialise cache with first four terms once */
    if (!cache) {
        cache = seq_alloc(4);
        if (!cache) return 0;           /* fallback on allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)26;
        cache[2] = (seq_t)447;
        cache[3] = (seq_t)6484;
        cache_size = 4;
    }

    /* return already computed term */
    if ((idx_t)n < cache_size)
        return cache[n];

    /* extend cache to include term n */
    cache = seq_realloc(cache, cache_size, (size_t)n + 1);
    if (!cache) return 0;               /* fallback on reallocation failure */

    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        seq_t term = (seq_t)26  * cache[i - 1]
                   - (seq_t)229 * cache[i - 2]
                   + (seq_t)816 * cache[i - 3]
                   - (seq_t)1008 * cache[i - 4];
        cache[i] = term;
    }
    cache_size = (size_t)n + 1;
    return cache[n];
}
