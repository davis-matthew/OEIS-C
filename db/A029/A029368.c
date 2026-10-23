/*
 * A029368  Expansion of 1/((1-x^4)*(1-x^7)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   alloc_seq_array, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029368 – number of partitions of n into 4, 7, 11, 12.
   Recurrence (unrolled as coin‑change DP):
     a(0) = 1
     a(n) = Σ_{p∈{4,7,11,12}, n≥p} a(n-p)   for n>0
   This is the standard “unbounded knapsack” DP that counts
   combinations of the parts irrespective of order.  It matches the
   generating function 1/((1−x⁴)(1−x⁷)(1−x¹¹)(1−x¹²)). */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_cache.h"

static seq_t *cache = NULL;
static idx_t  cache_cap = 0;

static const idx_t parts[4] = {4, 7, 11, 12};

/* Extend the cache to cover at least indices 0..n inclusive.  */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;                        /* already computed */

    /* Allocate a fresh array for the new size. */
    idx_t new_cap = n + 1;
    seq_t *new_cache = alloc_seq_array(new_cap);

    /* Initialise to zero. */
    for (idx_t i = 0; i < new_cap; ++i)
        new_cache[i] = 0;

    /* Base case. */
    new_cache[0] = 1;

    /* Unbounded‑knapsack DP: outer loop over parts, inner over n. */
    for (size_t pi = 0; pi < sizeof(parts)/sizeof(parts[0]); ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i < new_cap; ++i)
            new_cache[i] += new_cache[i - p];
    }

    /* Replace the old cache. */
    if (cache)
        free_cache(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
