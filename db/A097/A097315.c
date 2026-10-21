/*
 * A097315  Pell equation solutions (3*b(n))^2 - 10*a(n)^2 = -1 with b(n) =
 *          A097314(n), n >= 0.
 * https://oeis.org/A097315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 38*a(n-1) - a(n-2), a(0) = 1, a(1) = 37
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute terms of A097315.  Cache results to handle out‑of‑order
   requests efficiently.  The recurrence is linear, so we can generate
   any n in O(n) time and O(n) space.  The cache is grown only
   as needed. */
static seq_t *cache = NULL;   /* array of cached values */
static idx_t cache_end = 0;   /* number of allocated entries (cache[0..cache_end-1]) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)37;

    /* Grow cache to hold indices up to n, if necessary */
    if (cache_end <= n) {
        idx_t old_end = cache_end;
        idx_t new_end = n + 1;

        /* Allocate or reallocate memory */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_end * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_end * sizeof(seq_t));
        }

        /* Ensure base values are present */
        if (old_end < 2) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)37;
            old_end = 2;
        }

        /* Compute missing terms using the recurrence */
        for (idx_t i = old_end; i <= n; ++i) {
            cache[i] = (seq_t)38 * cache[i - 1] - cache[i - 2];
        }

        cache_end = new_end;
    }

    return cache[n];
}
