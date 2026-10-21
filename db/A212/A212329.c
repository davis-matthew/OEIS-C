/*
 * A212329  Expansion of x*(5+x)/(1-7*x+7*x^2-x^3).
 * https://oeis.org/A212329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A212329:
 * a(1) = 0
 * a(2) = 5
 * a(3) = 36
 * a(n) = 7*a(n-1) - 7*a(n-2) + a(n-3)   for n ≥ 4
 *
 * The implementation uses a static cache so that calls with
 * arbitrary n and in any order remain fast.
 */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Immediate handling of the first three terms. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)36;

    /* Static cache of previously computed values. */
    static seq_t *cache   = NULL;   /* array holding a(1)...a(m) : cache[0] = a(1) */
    static size_t cap    = 0;       /* allocated capacity */
    static size_t len    = 0;       /* number of initialized terms */

    /* Allocate or grow the cache as needed. */
    if (cap < (size_t)n) {
        size_t new_cap = cap ? cap << 1U : 4U;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        /* realloc must be cast for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort (the caller will handle it). */
            return 0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialize the cache with the first three terms if not already done. */
    if (len < 3) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)36;
        len = 3;
    }

    /* Compute missing terms up to a(n). */
    for (size_t i = len; i < (size_t)n; ++i) {
        cache[i] = (seq_t)7 * cache[i - 1]          /* 7*a(i)   */
                   - (seq_t)7 * cache[i - 2]          /* -7*a(i-1)*/
                   + cache[i - 3];                    /* + a(i-2) */
    }
    len = (size_t)n;  /* we now have up to a(n) computed */

    return cache[n - 1];  /* a(1) is stored at cache[0] */
}
