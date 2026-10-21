/*
 * A061418  a(n) = floor(a(n-1)*3/2) with a(1) = 2.
 * https://oeis.org/A061418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(a(n-1) * 3 / 2), a(1) = 2
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 54, u64 n <= 109,
 *            u128 n <= 218, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence uses only integer addition and right‑shift, so it works for
   any unsigned integer type (fixed‑width or arbitrary‑precision). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) is undefined; return 0 as a neutral value. */
    if (n == 0) return (seq_t)0;

    /* Static cache: 1‑based indexing. */
    static seq_t *cache = NULL;
    static idx_t cap     = 0;   /* current allocated capacity (>= 1) */
    static idx_t max_n   = 0;   /* last computed index */

    /* Initialise cache with first term. */
    if (cap == 0) {
        cap   = 4; /* initial capacity */
        cache = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed; return 0 */
        cache[1] = (seq_t)2;          /* a(1) = 2 */
        max_n   = 1;
    }

    /* Grow cache if n exceeds current capacity. */
    if (n > max_n) {
        /* Ensure the array is large enough. */
        while (n >= cap) {
            idx_t newcap = cap << 1;                /* double the size */
            seq_t *tmp   = (seq_t *)realloc(cache,
                                             (newcap + 1) * sizeof(seq_t));
            if (!tmp) return (seq_t)0;              /* allocation failed */
            cache = tmp;
            cap   = newcap;
        }

        /* Compute new terms up to n (iteratively). */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t prev  = cache[i - 1];
            seq_t half  = prev >> 1;      /* floor(prev / 2) */
            cache[i] = prev + half;       /* floor(prev * 3 / 2) */
        }
        max_n = n;
    }

    return cache[n];
}
