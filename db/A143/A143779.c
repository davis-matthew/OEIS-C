/*
 * A143779  a(n+1) = a(n)^2 - n^2, a(1) = 0.
 * https://oeis.org/A143779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 6, i32 n <= 7, i64 n <= 9, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence: a(1) = 0
 *            a(n+1) = a(n)^2 - n^2   (n ≥ 1)
 */

seq_t a(idx_t n) {
    if (n < 1)        /* undefined indices – return 0 safely */
        return (seq_t)0;

    /* Static cache: cache[0] holds a(1), cache[1] holds a(2), … */
    static seq_t *cache   = NULL;
    static idx_t  len     = 0;   /* number of cached terms */
    static size_t cap     = 0;   /* allocated capacity */

    /* Expand the cache if necessary */
    if (len < n) {
        size_t needed = (size_t)n;

        /* Grow capacity */
        if (cap < needed) {
            size_t new_cap = cap ? cap * 2 : 16;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }

        /* Initialize the first element if the cache is empty */
        if (len == 0) {
            cache[0] = 0;               /* a(1) */
            len = 1;
        }

        /* Compute missing terms */
        for (idx_t i = len; i < n; ++i) {   /* i is zero‑based index for a(i+1) */
            seq_t prev = cache[i - 1];      /* a(i)            */
            seq_t k    = (seq_t)i;          /* n in recurrence */
            cache[i] = prev * prev - k * k; /* a(i+1) = a(i)^2 - i^2 */
        }
        len = n;
    }

    return cache[n - 1];   /* nth term (1‑based index) */
}
