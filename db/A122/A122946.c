/*
 * A122946  a(0)=a(1)=0, a(2)=2; for n >= 3, a(n) = a(n-1) + 4*a(n-3).
 * https://oeis.org/A122946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 33, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=a(1)=0, a(2)=2; for n>=3 a(n)=a(n-1)+4*a(n-3) */

#include <stdlib.h>   /* malloc, free */

/* Static cache to support out‑of‑order calls */
static seq_t *cache   = NULL; /* pointer to cached values          */
static size_t cache_len = 0;   /* number of initialized entries     */
static size_t cache_cap = 0;   /* allocated capacity                */

/* Grow the cache to hold indices up to idx (inclusive) */
static void grow_cache(size_t idx)
{
    if (idx < cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= idx) {
        new_cap = new_cap * 2 + 1;  /* ensure we exceed idx */
    }

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (new_cache == NULL) {
        /* Allocation failed – aborting is acceptable for this build */
        abort();
    }

    /* Copy existing values */
    for (size_t i = 0; i < cache_len; ++i) {
        new_cache[i] = cache[i];
    }

    free(cache);
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Compute the nth term of the sequence, with memoisation. */
seq_t a(idx_t n)
{
    /* Convert index to size_t for array indexing.  The problem guarantees
       that n is modest (≤ 10000) for performance tests, so overflow is
       not an issue on the expected build platforms. */
    size_t idx = (size_t)n;

    /* Ensure the cache can address idx */
    grow_cache(idx);

    /* Compute missing values up to idx */
    for (size_t i = cache_len; i <= idx; ++i) {
        seq_t val;
        if (i == 0u || i == 1u) {
            val = 0;
        } else if (i == 2u) {
            val = 2;
        } else {
            /* a(i) = a(i-1) + 4 * a(i-3) */
            val = cache[i - 1];
            val = val + (cache[i - 3] * 4);
        }
        cache[i] = val;
    }

    cache_len = idx + 1;
    return cache[idx];
}
