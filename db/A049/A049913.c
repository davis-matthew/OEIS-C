/*
 * A049913  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m =
 *          2^(p+1) + 2 - n and p is the unique integer such that 2^p < n - 1
 *          <= 2^(p+1), starting with a(1) = 1, a(2) = 2, and a(3) = 4.
 * https://oeis.org/A049913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(1)=1, a(2)=2, a(3)=4, and for n>=4
 *
 *   a(n) = (a(1)+a(2)+...+a(n-1)) - a(m)
 *
 * where m = 2^(p+1) + 2 - n and p is the unique integer such that
 * 2^p < n-1 <= 2^(p+1).  The relation is implemented by
 * computing the cumulative sum of the sequence and the index m
 * as 2^(floor_log2(n-2)+1) + 2 - n.
 *
 * The routine memoises previously computed values so that calls
 * with arbitrary n (not necessarily monotone) still run O(n).
 */

#include <stdlib.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* Static cache and cumulative sum */
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;    /* number of terms stored (>=3 after init) */
    static idx_t cache_cap = 0;      /* current allocation */
    static seq_t cum_sum = 0;        /* sum of cache[1..cached_size] */

    /* Guard against calls with n < 1 */
    if (n < 1)
        return 0;

    /* Initialisation on first call or when cache too small */
    if (cached_size == 0) {
        /* Allocate some space; reserve index 0 for convenience */
        cache_cap = 64;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);

        /* Set known first terms */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;

        cached_size = 3;
        cum_sum = cache[1] + cache[2] + cache[3];  /* 7 */
    }
    else if (n > cache_cap) {
        /* Grow cache to accommodate requested n */
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If requested value already computed, return it */
    if (n <= cached_size)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t k = cached_size + 1; k <= n; ++k) {
        /* Find p such that 2^p < k-1 <= 2^(p+1).  For k-1 a power of two,
         * this is p = floor_log2(k-2).  For other k it is the same. */
        idx_t p = floor_log2_uint64(k - 2);

        /* Compute m = 2^(p+1) + 2 - k */
        idx_t m = ((idx_t)1 << (p + 1)) + 2 - k;

        /* a(k) = sum of previous terms minus a(m) */
        seq_t ak = cum_sum - cache[m];

        /* Store and update cumulative sum */
        cache[k] = ak;
        cum_sum += ak;
    }

    /* Update cached size */
    cached_size = n;

    return cache[n];
}
