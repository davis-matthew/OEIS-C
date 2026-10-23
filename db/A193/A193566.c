/*
 * A193566  Unhappy numbers which enter the cycle (4, 16, 37, 58, 89, 145, 42,
 *          20) at 16.
 * https://oeis.org/A193566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   cache_sum_digit_squares, enters_at_16
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Unhappy numbers reaching the cycle (4,16,37,58,89,145,42,20)
   by first hitting 16.  The function searches consecutively for integers
   satisfying the condition and caches the results for future calls. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/cache_sum_digit_squares.h"
#include "../utils/enters_at_16.h"

/* Return the n-th term of the sequence, 1‑based indexing.  The function
   maintains a static cache of already found terms and expands it as
   needed.  The cache is grown by doubling its capacity to keep the
   overall time at O(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */

    /* Static cache of computed terms. */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;   /* number of cached terms */
    static idx_t cache_cap   = 0;   /* allocated capacity */

    /* Expand the cache until it contains at least n terms. */
    if (n > cache_len) {
        /* Allocate initial capacity if necessary. */
        if (cache_cap == 0) {
            cache_cap = 256;
            cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        }

        /* Start searching from the next integer after the last cached term. */
        seq_t candidate = (cache_len == 0) ? (seq_t)1 : cache[cache_len - 1] + (seq_t)1;

        /* Generate missing terms until we have n of them. */
        while (cache_len < n) {
            /* Find the next number that satisfies the condition. */
            while (!enters_at_16(candidate))
                candidate++;

            /* Grow the cache if full. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
                if (!tmp) {               /* allocation failed – abort safely */
                    return 0;
                }
                cache = tmp;
                cache_cap = (idx_t)new_cap;
            }

            /* Store the found term. */
            cache[cache_len++] = candidate;
            candidate++;                     /* next candidate for next search */
        }
    }

    return cache[n - 1];
}
