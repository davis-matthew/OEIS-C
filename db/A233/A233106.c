/*
 * A233106  Number of n X 1 0..5 arrays with no element x(i,j) adjacent to
 *          value 5-x(i,j) horizontally or vertically, top left element zero,
 *          and 1 appearing before 2 3 and 4, and 2 appearing before 3 in row
 *          major order.
 * https://oeis.org/A233106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=6, a(n)=9*a(n-1)-23*a(n-2)+15*a(n-3) for
 *            n>=4 (OEIS A233106)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   alloc_seq_array, mul_small, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/mul_small.h"

/* dynamic‑programming cache for previously computed terms */
static seq_t *cache = NULL;   /* sequence of cached terms */
static idx_t cache_len = 0;   /* number of valid terms in cache */
static idx_t cache_cap = 0;   /* allocated capacity of cache */

/* grow the cache to at least new_cap entries */
static void grow_cache(idx_t new_cap)
{
    seq_t *new_cache = alloc_seq_array(new_cap);
    if (cache != NULL) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        seq_free_array(cache);
    }
    cache     = new_cache;
    cache_cap = new_cap;
}

/* OEIS A233106 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1 */

    /* initialise cache if empty */
    if (cache == NULL) {
        cache_cap = 3;                     /* at least 3 slots needed */
        cache     = alloc_seq_array(cache_cap);
        cache[0] = (seq_t)1;               /* a(1) */
        cache[1] = (seq_t)2;               /* a(2) */
        cache[2] = (seq_t)6;               /* a(3) */
        cache_len = 3;
    }

    /* if we already know the required term, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* enlarge the cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        while (new_cap < n) new_cap *= 2;
        grow_cache(new_cap);
    }

    /* compute missing terms up to n (recurrence needs 3 previous terms) */
    for (idx_t i = cache_len; i < n; ++i) {
        /* i corresponds to index n = i+1 in 1‑based sequence */
        seq_t term1 = mul_small(cache[i - 1], 9);   /* 9*a(n-1) */
        seq_t term2 = mul_small(cache[i - 2], 23);  /* 23*a(n-2) */
        seq_t term3 = mul_small(cache[i - 3], 15);  /* 15*a(n-3) */
        cache[i] = term1 - term2 + term3;
    }
    cache_len = n;

    return cache[n - 1];
}
