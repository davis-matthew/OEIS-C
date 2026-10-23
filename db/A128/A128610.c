/*
 * A128610  a(0)=a(1)=1. For n >= 2, a(n) = a(n-2) + a(n-1) + (number of terms
 *          from among {a(0),a(1),a(2),...a(n-1)} which are <= n).
 * https://oeis.org/A128610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1; for n>=2 a(n) = a(n-2) + a(n-1) + (# of k<n with
 *            a(k)<=n)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms, growing as needed */
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* allocated slots */
    static idx_t last   = 0;     /* largest index computed (inclusive) */
    static size_t cnt_le = 0;    /* number of a[k] <= current i during construction */

    /* Initialise on first call */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        last = 1;
        cnt_le = 0;      /* will be updated during construction */
    }

    /* If the requested index is already computed, return it */
    if (n <= last) {
        return cache[(size_t)n];
    }

    /* Ensure capacity and compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* Extend storage if necessary */
        if ((size_t)i + 1 > cap) {
            size_t new_cap = cap * 2;
            if (new_cap < (size_t)i + 2) new_cap = (size_t)i + 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cap = new_cap;
        }

        /* Update cnt_le: number of a[k] (k<i) that are <= i */
        while (cnt_le < (size_t)i && cache[cnt_le] <= (seq_t)i)
            ++cnt_le;

        /* Compute a(i) using the recurrence */
        cache[(size_t)i] = cache[(size_t)(i-2)]
                          + cache[(size_t)(i-1)]
                          + (seq_t)cnt_le;
    }

    last = n;
    return cache[(size_t)n];
}
