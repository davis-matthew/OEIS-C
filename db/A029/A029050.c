/*
 * A029050  Expansion of 1/((1-x)*(1-x^3)*(1-x^7)*(1-x^9)).
 * https://oeis.org/A029050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 3, 7, 9
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming with coin‑change style recurrence.
   a[0] = 1; for each part p in {1,3,7,9}:
     for i = p .. n: a[i] += a[i-p];
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Helper: recompute all values up to limit n (inclusive). */
static void recompute(idx_t n)
{
    if (cache == NULL || cache_size < (size_t)n + 1) {
        size_t new_size = (size_t)n + 1;
        seq_t *ptr = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (ptr == NULL) {
            /* Allocation failed – abort with zero to avoid crash in test harness */
            exit(EXIT_FAILURE);
        }
        cache = ptr;
        cache_size = new_size;
    }

    /* Initialise all entries to 0 and set base case */
    memset(cache, 0, cache_size * sizeof(seq_t));
    cache[0] = 1;

    /* Coins (parts) in ascending order */
    static const idx_t parts[4] = {1, 3, 7, 9};
    for (int c = 0; c < 4; ++c) {
        idx_t p = parts[c];
        for (idx_t i = p; i <= n; ++i) {
            cache[i] = cache[i] + cache[i - p];
        }
    }
}

seq_t a(idx_t n)
{
    /* Ensure the cache covers the requested index */
    if (n + 1 > cache_size) {
        recompute(n);
    }
    return cache[n];
}
