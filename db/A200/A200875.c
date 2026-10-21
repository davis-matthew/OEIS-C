/*
 * A200875  Number of 0..n arrays x(0..6) of 7 elements without any interior
 *          element greater than both neighbors or less than both neighbors.
 * https://oeis.org/A200875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 55, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A200875: Number of 0..n arrays x(0..6) of 7 elements
 * without any interior element greater than both neighbors
 * or less than both neighbors.
 *
 * Recurrence (valid for n > 8):
 *   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3)
 *         - 70*a(n-4) + 56*a(n-5) - 28*a(n-6)
 *         + 8*a(n-7) - a(n-8)
 * Initial values:
 *   a(1)=42, a(2)=343, a(3)=1528, a(4)=4917, a(5)=12890,
 *   a(6)=29325, a(7)=60112, a(8)=113745
 *
 * This implementation caches computed values in a static array
 * and expands the array as needed.  Operations are performed
 * using seq_t arithmetic, so the function works for any unsigned
 * seq_t, whether it is a plain integer type, 128‑bit, or arbitrary
 * precision class.
 *
 * No external dependencies other than stdlib.h are required.
 */

#include <stdlib.h>
#include <stddef.h>

/* The function a(n) provides the nth term of A200875,
 * where the offset is **1**; that is, a(1) returns the
 * first listed term (42).  For n < 1 the function returns 0. */
seq_t a(idx_t n)
{
    /* Static state for memoisation. */
    static seq_t *cache = NULL;   /* 1‑indexed array: cache[i] = a(i) */
    static size_t cap    = 0;     /* allocated capacity (indices 0..cap-1) */
    static idx_t cached_max = 0;  /* index of the largest valid cache entry */

    if (n < 1)
        return (seq_t)0;

    /* Expand the cache if the requested index is outside the current capacity. */
    if (n >= cap) {
        size_t new_cap = n + 1;          /* ensure space for indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* memory allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Return the value immediately if it has already been computed. */
    if (n <= cached_max)
        return cache[n];

    /* Initialise the first eight terms once. */
    if (cached_max < 8) {
        if (cap < 9) {                 /* indices 0..8 are needed */
            seq_t *tmp = (seq_t *)realloc(cache, 9 * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap   = 9;
        }
        cache[1] = (seq_t)42;
        cache[2] = (seq_t)343;
        cache[3] = (seq_t)1528;
        cache[4] = (seq_t)4917;
        cache[5] = (seq_t)12890;
        cache[6] = (seq_t)29325;
        cache[7] = (seq_t)60112;
        cache[8] = (seq_t)113745;
        cached_max = 8;
    }

    /* Compute any missing terms using the recurrence. */
    for (idx_t k = cached_max + 1; k <= n; ++k) {
        seq_t high = (seq_t)8  * cache[k - 1]
                   + (seq_t)56 * cache[k - 3]
                   + (seq_t)56 * cache[k - 5]
                   + (seq_t)8  * cache[k - 7];

        seq_t low = (seq_t)28 * cache[k - 2]
                  + (seq_t)70 * cache[k - 4]
                  + (seq_t)28 * cache[k - 6]
                  + cache[k - 8];

        cache[k] = high - low;
    }

    cached_max = n;
    return cache[n];
}
