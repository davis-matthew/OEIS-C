/*
 * A099587  a(n) = coefficient of x in (1+x)^n mod (1+x^4).
 * https://oeis.org/A099587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n‑1) - 6*a(n‑2) + 4*a(n‑3) - 2*a(n‑4) a(0) = 0, a(1)
 *            = 1, a(2) = 2, a(3) = 3, a(4) = 4
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 38, i64 n <= 72, i128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function is thread‑unsafe; it is intended for use in single‑threaded
   OEIS evaluation code.  It caches computed terms in a static array to
   allow non‑monotonic queries.  The cache is expanded with realloc(), hence
   the explicit cast to (seq_t *) for C++ compatibility. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* In‑function static cache for memoization. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of entries currently stored */

    /* Convert index to size_t safely (index is unsigned). */
    size_t need = (size_t)n + 1;

    /* Grow cache if necessary. */
    if (cache_len < need) {
        /* realloc() with explicit cast for C++ */
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort the program (simply abort, not printf). */
            abort();
        }
        cache = tmp;

        /* Compute all missing terms up to n. */
        for (size_t i = cache_len; i < need; ++i) {
            if (i == 0)                cache[i] = (seq_t)0;
            else if (i == 1)            cache[i] = (seq_t)1;
            else if (i == 2)            cache[i] = (seq_t)2;
            else if (i == 3)            cache[i] = (seq_t)3;
            else if (i == 4)            cache[i] = (seq_t)4;
            else {
                /* recurrence: 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - 2*a(n-4) */
                seq_t t1 = cache[i-1];
                seq_t t2 = cache[i-2];
                seq_t t3 = cache[i-3];
                seq_t t4 = cache[i-4];
                cache[i] = (seq_t)(4*t1 - 6*t2 + 4*t3 - 2*t4);
            }
        }
        cache_len = need;
    }

    return cache[(size_t)n];
}
