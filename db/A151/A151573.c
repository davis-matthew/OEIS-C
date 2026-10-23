/*
 * A151573  a(0)=1, a(1)=1; a(2^i + j) = a(j) + 2*a(j+1) for 0 <= j < 2^i.
 * https://oeis.org/A151573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1; a(2^i + j) = a(j) + 2*a(j+1) for 0 <= j <
 *            2^i
 * Range:     u8 n <= 59, u16 n <= 2040, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;     /* cache for computed values   */
    static size_t cap   = 0;        /* current capacity of cache   */
    static idx_t  max_computed = 0; /* highest index already computed */

    /* Ensure the cache is large enough. */
    ensure_cache(n, &cache, &cap);

    /* Base cases – they are safe to re‑assign on each call. */
    if (cap >= 2) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
    }

    /* If this is the first call, initialise the tracker. */
    if (max_computed < 1)
        max_computed = 1;

    /* Compute any missing values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Highest power of two that does not exceed i. */
        idx_t pow2 = ((idx_t)1) << floor_log2_uint64(i);
        idx_t j = i - pow2;
        cache[i] = cache[j] + (seq_t)2 * cache[j + 1];
    }
    max_computed = n;

    return cache[n];
}
