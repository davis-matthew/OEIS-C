/*
 * A033648  Trajectory of 3 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 3; a(n+1) = a(n) + reverse_decimal(a(n))
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n) {
    /* Static cache to store computed terms up to the largest requested index. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* number of terms currently stored */
    static size_t cache_cap = 0; /* allocated capacity */

    /* Ensure we have capacity for at least n+1 terms. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 1 : cache_cap * 2;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* In a real implementation we would handle allocation failure. */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first term if needed. */
    if (cache_len == 0) {
        cache[0] = 3;          /* a(0) = 3 */
        cache_len = 1;
    }

    /* Compute terms up to index n if they are not yet cached. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t rev  = reverse_decimal(prev);
        cache[i] = prev + rev;
    }
    cache_len = (size_t)n + 1;   /* now we have terms 0..n */

    return cache[(size_t)n];
}
