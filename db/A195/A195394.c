/*
 * A195394  a(n) = (10*n)!
 * https://oeis.org/A195394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n)!
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

/* Static cache of factorial values */
static seq_t *fac_cache = NULL;          /* array of (10*i)! for i = 0 .. cache_cap-1 */
static idx_t  cache_cap  = 0;            /* allocated size of fac_cache          */
static idx_t  cache_size = 0;            /* number of elements actually computed */

static void ensure_cache(idx_t n)
{
    /* Ensure we have enough space for indices up to n */
    if (cache_size == 0) {
        /* Initial allocation (at least one slot) */
        cache_cap = (n >= 0) ? (size_t)(n + 1) : 1;
        fac_cache = seq_alloc_array(cache_cap);
        fac_cache[0] = (seq_t)1;          /* 0! = 1 */
        cache_size = 1;
    }

    if (n >= cache_cap) {
        /* Grow the cache to accommodate index n */
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = seq_alloc_array(new_cap);
        for (idx_t i = 0; i < cache_size; ++i) {
            new_cache[i] = fac_cache[i];
        }
        free_seq_array(fac_cache);
        fac_cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing factorials up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1; /* (10*0)! = 0! = 1 */
        } else {
            val = fac_cache[i - 1];                     /* (10*(i-1))!  */
            unsigned long long start = 10ULL * (i - 1) + 1; /* start of next block */
            unsigned long long end   = 10ULL * i;          /* end of next block   */
            for (unsigned long long m = start; m <= end; ++m) {
                val *= (seq_t)m;
            }
        }
        fac_cache[i] = val;
    }

    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return fac_cache[n];
}
