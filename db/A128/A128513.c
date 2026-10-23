/*
 * A128513  a(1)=2, a(n) = 1 + (smallest integer > a(n-1) with the same number
 *          of divisors as a(n-1)).
 * https://oeis.org/A128513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   divisor_count_u64, free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A128513:  a(1)=2, a(n)=1 + smallest integer > a(n-1) having the same divisor count as a(n-1) */
#include <stddef.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache      = NULL;   /* 0‑based array: cache[i] = a(i+1) */
static size_t cache_len  = 0;      /* number of values stored */
static size_t cache_cap  = 0;      /* allocated capacity */

/* guarantee that the cache can hold at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;

    size_t new_cap = needed * 2;
    seq_t *new_arr = seq_alloc_new_array(new_cap);

    /* copy existing values */
    for (size_t i = 0; i < cache_len; ++i)
        new_arr[i] = cache[i];

    /* free old memory */
    if (cache) free_seq_array(cache);

    cache      = new_arr;
    cache_cap  = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, but guard against accidental 0‑based access */
        return (seq_t)2;

    size_t idx = (size_t)n;

    /* initialise cache with a(1) */
    if (cache_len == 0) {
        ensure_capacity(1);
        cache[0] = (seq_t)2;
        cache_len = 1;
    }

    /* generate terms up to a(n) if necessary */
    if (idx > cache_len) {
        ensure_capacity(idx);
        for (size_t i = cache_len; i < idx; ++i) {
            seq_t prev = cache[i - 1];
            uint64_t dprev = divisor_count_u64((uint64_t)prev);

            /* find the smallest integer > prev with dprev divisors */
            uint64_t cand = (uint64_t)prev + 1;
            while (divisor_count_u64(cand) != dprev)
                ++cand;

            cache[i] = (seq_t)(cand + 1);
        }
        cache_len = idx;
    }

    return cache[idx - 1];
}
