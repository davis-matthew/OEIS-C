/*
 * A118237  Start with 14 and repeatedly reverse the digits and add 1 to get
 *          the next term.
 * https://oeis.org/A118237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 14, a(n) = rev_decimal(a(n-1)) + 1 (n > 1)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented with memoization. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"   /* seq_t reverse_digits(seq_t x) */

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache     = NULL;   /* 1‑based array; cache[0] unused */
    static size_t cap       = 0;      /* allocated size of cache array */
    static idx_t max_cached = 0;      /* highest index that has been computed */

    /* Sanity: n >= 1 (outside specification behaviour undefined for n < 1) */
    if (n == 0) return 0;

    /* Allocate or resize the cache array if needed */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) return 0;            /* allocation failed; return zero */
        cache = new_cache;
        cap   = needed;
    }

    /* Compute terms up to index n if not already computed */
    if (max_cached < n) {
        if (max_cached == 0) {
            cache[1] = (seq_t)14;            /* a(1) = 14 */
            max_cached = 1;
        }
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            cache[i] = reverse_digits(cache[i - 1]) + (seq_t)1;
        }
        max_cached = n;
    }

    return cache[n];
}
