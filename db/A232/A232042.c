/*
 * A232042  Number of n X 3 0..1 arrays with no element less than a strict
 *          majority of its horizontal and antidiagonal neighbors.
 * https://oeis.org/A232042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 4*a(n-1) + 9*a(n-2) - a(n-3) - 6*a(n-4) for n > 5, *
 *            with initial values a(1)=4, a(2)=15, a(3)=80, a(4)=446,
 *            a(5)=2477
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of OEIS sequence A232042.  n >= 1. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;                /* undefined, but safe */

    /* Static cache to memoize computed terms. */
    static seq_t *cache = NULL;                /* array of terms */
    static size_t cap = 0;                     /* allocated capacity */
    static idx_t last = 0;                     /* highest index computed (1‑based) */

    /* On first call, initialize cache with base terms. */
    if (cache == NULL) {
        cap = 8;                               /* some initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* OOM fallback */

        cache[0] = (seq_t)4;      /* a(1) */
        cache[1] = (seq_t)15;     /* a(2) */
        cache[2] = (seq_t)80;     /* a(3) */
        cache[3] = (seq_t)446;    /* a(4) */
        cache[4] = (seq_t)2477;   /* a(5) */
        last = 5;
    }

    /* If requested term is already cached, return it. */
    if ((size_t)n <= (size_t)last) {
        return cache[(size_t)n - 1];
    }

    /* Ensure we have enough space for terms up to n. */
    size_t needed = (size_t)n;
    if (cap < needed) {
        size_t newcap = cap ? cap * 2 : 8;
        if (newcap < needed) newcap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* OOM fallback */
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms using recurrence. */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t val = 4 * cache[k - 2] +        /* 4*a(k-1) */
                    9 * cache[k - 3] -        /* 9*a(k-2) */
                    cache[k - 4] -            /* -a(k-3)   */
                    6 * cache[k - 5];        /* -6*a(k-4) */
        cache[k - 1] = val;
    }
    last = n;

    return cache[(size_t)n - 1];
}
