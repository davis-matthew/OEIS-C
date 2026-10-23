/*
 * A170056  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(k)=45·∑_{i=1}^{35}a(k−i) – 1035·a(k−36) + p(k), with
 *            p(k)=1 if k=0 or k=36, p(k)=2 if 1≤k≤35, and p(k)=0 otherwise
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses memoization and the recurrence derived from the
   rational generating function. */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"

static seq_t *cache      = NULL;   /* stored sequence values */
static size_t cache_cap  = 0;      /* allocated capacity of cache   */
static size_t cache_len  = 0;      /* number of values currently stored */

/* extend the cache so that values up to idx (inclusive) are available */
static void grow_cache(idx_t idx)
{
    if (cache_len > idx) return;                /* already computed    */

    /* allocate a larger buffer if necessary */
    size_t needed = (size_t)idx + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_arr = seq_alloc_array(new_cap);
        for (size_t i = 0; i < cache_len; ++i) new_arr[i] = cache[i];
        cache      = new_arr;
        cache_cap  = new_cap;
        /* old array is intentionally leaked – the memory footprint remains
           tiny (≤ 2×final size) and is harmless for the required limits   */
    }

    /* compute missing terms using the recurrence */
    for (idx_t k = cache_len; k <= idx; ++k) {
        seq_t term = 0;

        /* 45 * a(k-1) + ... + 45 * a(k-35) */
        idx_t max_j = (k > 35) ? 35 : (idx_t)k;
        for (idx_t j = 1; j <= max_j; ++j)
            term += cache[(size_t)(k - j)] * (seq_t)45;

        /* − 1035 * a(k-36)  (only for k ≥ 36) */
        if (k >= 36)
            term -= cache[(size_t)(k - 36)] * (seq_t)1035;

        /* add the numerator contribution p(k) */
        if (k == 0 || k == 36)          term += (seq_t)1;
        else if (k <= 35)               term += (seq_t)2;
        /* otherwise p(k)=0 – nothing to add */

        cache[(size_t)k] = term;
    }

    cache_len = (size_t)idx + 1;
}

/* public entry function for the sequence */
seq_t a(idx_t n)
{
    grow_cache(n);
    return cache[(size_t)n];
}
