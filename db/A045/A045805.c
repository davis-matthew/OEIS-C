/*
 * A045805  5-ish numbers (end in 51, 57, 93, 99).
 * https://oeis.org/A045805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045805 (5‑ish numbers)
   Formula: a(n) = a(n-1) + a(n-4) - a(n-5), n >= 6
   Initial values: a(1)=51, a(2)=57, a(3)=93, a(4)=99, a(5)=151 */
#include <stdlib.h>

/* Global cache for memoization; 1‑based indexing -> cache[i-1] = a(i) */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored */

/* Compute a(n) for n >= 1, any order of calls */
seq_t a(idx_t n)
{
    /* Ensure cache has at least n terms */
    if ((size_t)n > SIZE_MAX) { /* index too large for array operations */
        /* In practice this should never happen for the test range (n <= 10000) */
        return (seq_t)0;
    }
    size_t required = (size_t)n;
    if (required > cache_len) {
        /* Reallocate cache to hold the new larger size */
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        /* Fill newly allocated slots */
        for (size_t i = cache_len; i < required; ++i) {
            seq_t val;
            /* Map 1‑based index i+1 to value */
            if (i == 0u) {
                val = (seq_t)51;
            } else if (i == 1u) {
                val = (seq_t)57;
            } else if (i == 2u) {
                val = (seq_t)93;
            } else if (i == 3u) {
                val = (seq_t)99;
            } else if (i == 4u) {
                val = (seq_t)151;
            } else {
                /* Recurrence: a(i+1) = a(i) + a(i-3) - a(i-4) */
                /* indices: cache[i] = a(i+1),       i-1 = a(i),   */
                /*           i-4 = a(i-3),           i-5 = a(i-4) */
                val = cache[i - 1] + cache[i - 4] - cache[i - 5];
            }
            cache[i] = val;
        }
        cache_len = required;
    }
    /* Return the requested term */
    return cache[n - 1];
}
