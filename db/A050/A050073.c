/*
 * A050073  a(n) = |a(n-1) - a(m)| for n >= 4, where m = n - 1 - 2^p and p is
 *          the unique integer such that 2^p < n - 1 <= 2^(p+1), starting with
 *          a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A050073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |a(n-1) - a(n-1-2^p)| for n ≥ 4, where p is the unique
 *            integer such that 2^p < n-1 ≤ 2^(p+1). Initial values: a(1) =
 *            a(2) = a(3) = 1
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   fl_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/fl_log2_uint64.h"

static seq_t *cache = NULL;      /* 1‑based: cache[0] = a(1) */
static idx_t cache_size = 0;    /* number of terms currently stored */

seq_t a(idx_t n) {
    /* Ensure we have space for at least n entries. */
    if (cache_size < n) {
        size_t needed = n;
        size_t capacity = cache_size ? cache_size : 16;
        while (capacity < needed) capacity <<= 1;

        seq_t *new_cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (!new_cache) { /* allocation failed – abort gracefully */
            return (seq_t)0;
        }
        cache = new_cache;

        /* Initialise new entries if needed */
        for (idx_t i = cache_size + 1; i <= needed; ++i) {
            if (i <= 3) {
                cache[i - 1] = (seq_t)1;
                continue;
            }
            /* Recurrence */
            idx_t k = i - 1;                       /* n-1 */
            /* p is floor(log2(k-1)) so that 2^p < k ≤ 2^(p+1) */
            int p = fl_log2_uint64((uint64_t)(k - 1));
            uint64_t pow2 = ((uint64_t)1) << p;    /* 2^p */
            idx_t m = k - (idx_t)pow2;             /* (n-1) - 2^p */

            seq_t val_prev   = cache[i - 2];        /* a(n-1)   */
            seq_t val_m      = cache[m - 1];        /* a(m)     */
            seq_t diff = (val_prev >= val_m) ? (val_prev - val_m)
                                             : (val_m - val_prev);
            cache[i - 1] = diff;
        }

        cache_size = n;
    }
    return cache[n - 1];
}
