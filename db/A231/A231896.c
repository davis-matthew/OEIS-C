/*
 * A231896  a(n) = 4*a(n-1) - a(n-2) with a(0) = 0, a(1) = 4.
 * https://oeis.org/A231896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS entry A231896
   Recurrence:  a(0) = 0,  a(1) = 4,
   a(n) = 4*a(n-1) - a(n-2)  for n ≥ 2
   (Non‑recursive implementation with memoisation.) */

#include <stddef.h>
#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Static cache for memoisation: cache[i] == a(i) */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* current size of cache   */

    /* Handle base cases directly. */
    if (n == 0) return 0;
    if (n == 1) return 4;

    /* Convert index to size_t for array access. */
    const size_t idx = (size_t)n;

    /* Initialise cache on first call. */
    if (cap == 0) {
        size_t init = 2;                 /* indices 0 and 1 are needed */
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure → 0 */
        cache[0] = 0;
        cache[1] = 4;
        cap     = init;
    }

    /* Grow cache if necessary. */
    if (idx >= cap) {
        size_t old_cap = cap;
        size_t new_cap = idx + 1;        /* need indices 0 … idx */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;             /* allocation failure → 0 */
        cache = tmp;
        cap   = new_cap;

        /* Fill the new entries using the recurrence. */
        for (size_t i = old_cap; i <= idx; ++i) {
            cache[i] = ((seq_t)4) * cache[i - 1] - cache[i - 2];
        }
    }

    return cache[idx];
}
