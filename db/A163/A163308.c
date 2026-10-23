/*
 * A163308  a(n) = 16*a(n-1) - 59*a(n-2) for n > 1; a(0) = 1, a(1) = 9.
 * https://oeis.org/A163308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 59*a(n-2) with a(0) = 1, a(1) = 9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* Thread‑safe on a single thread.  Cache grows lazily. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    /* Ensure we have at least two initial values in the cache. */
    if (cache == NULL) {
        cache_len = 2;
        cache = seq_alloc_array(cache_len);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
    }
    /* Handle small indices directly. */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* If already computed, return. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Need to extend the cache to contain a(n). */
    idx_t old_len = cache_len;
    idx_t needed  = n + 1;
    if (needed > old_len) {
        seq_t *new_cache = seq_alloc_array(needed);
        for (idx_t i = 0; i < old_len; ++i) {
            new_cache[i] = cache[i];
        }
        seq_free_array(cache);
        cache = new_cache;
        cache_len = needed;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = old_len; i <= n; ++i) {
        cache[i] = cache[i-1] * (seq_t)16 - cache[i-2] * (seq_t)59;
    }

    return cache[n];
}
