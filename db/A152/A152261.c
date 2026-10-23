/*
 * A152261  a(n) = ((9 + sqrt(5))^n + (9 - sqrt(5))^n)/2.
 * https://oeis.org/A152261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((9 + sqrt(5))^n + (9 - sqrt(5))^n)/2. Recurrence: a(0) =
 *            1, a(1) = 9, a(n) = 18*a(n‑1) – 76*a(n‑2) for n > 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

/* seq_t and idx_t are defined by the build environment. */
/* The function returns a(n) for OEIS sequence A152261. */
seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t  *cache = NULL;    /* array of terms   */
    static size_t  cap   = 0;       /* current capacity */
    static size_t  len   = 0;       /* number of filled terms */

    /* --- Initialise the cache with the first two terms. --- */
    if (!cache) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)           /* allocation failure – return 0 */
            return (seq_t)0;
        cache[0] = (seq_t)1;  /* a(0) */
        cache[1] = (seq_t)9;  /* a(1) */
        len = 2;
    }

    /* Convert the unsigned idx_t to a size_t for array indexing. */
    size_t idx = (size_t)n;

    /* --- Grow the cache enough to contain a(n). --- */
    if (idx >= len) {
        /* Ensure capacity.  Double the space until it is large enough. */
        if (idx + 1 > cap) {
            size_t newcap = cap ? cap : 1;
            while (newcap <= idx)
                newcap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;           /* allocation failure */
            cache = tmp;
            cap = newcap;
        }

        /* Compute all missing terms using the recurrence. */
        for (size_t i = len; i <= idx; ++i) {
            /* a(i) = 18*a(i-1) – 76*a(i-2) */
            cache[i] = (seq_t)18 * cache[i - 1] - (seq_t)76 * cache[i - 2];
        }
        len = idx + 1;
    }

    /* Return the requested term. */
    return cache[idx];
}
