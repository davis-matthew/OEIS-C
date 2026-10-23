/*
 * A128785  a(n) = n^2*6^n.
 * https://oeis.org/A128785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*6^n Recurrence: a(n) = 18*a(n-1) - 108*a(n-2) +
 *            216*a(n-3) initial values: a(0) = 0, a(1) = 6, a(2) = 144
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache to store computed terms */
    static seq_t *cache = NULL;      /* array of a(i) values */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_len = 0;     /* number of terms stored (>=0) */

    /* grow the cache if needed */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;          /* need indices 0..n */
        if (new_len > cache_cap) {
            size_t new_cap = new_len;
            if (!cache) {
                cache = seq_alloc(new_cap);
            } else {
                cache = seq_realloc(cache, cache_cap, new_cap);
            }
            cache_cap = new_cap;
        }
        /* compute and store new terms */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)0;
            } else if (i == 1) {
                val = (seq_t)6;
            } else if (i == 2) {
                val = (seq_t)144;          /* 2^2 * 6^2 */
            } else {
                /* recurrence */
                val = 18 * cache[i-1];
                val = val - 108 * cache[i-2];
                val = val + 216 * cache[i-3];
            }
            cache[i] = val;
        }
        cache_len = new_len;
    }
    return cache[n];
}
