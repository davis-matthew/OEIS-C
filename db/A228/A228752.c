/*
 * A228752  Number of n X 6 binary arrays with top left element equal to 1 and
 *          no two ones adjacent horizontally or antidiagonally.
 * https://oeis.org/A228752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1) - 112*a(n-2) + 217*a(n-3) - 157*a(n-4) +
 *            36*a(n-5) Initial values: a(1)=8, a(2)=119, a(3)=1712,
 *            a(4)=24246, a(5)=342207, a(6)=4823826 Computation uses a linear
 *            recurrence; values are cached so that calls in any order are
 *            O(1) after the first cache fill
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Top‑left element must be 1, so the sequence is defined for n>=1. */
    if (n == 0)
        return 0;   /* undefined, but harmless return */

    /* Cached values: index i stored at cache[i] (1‑based). */
    static seq_t *cache   = NULL;
    static size_t cache_sz = 0;          /* current allocated size */

    /* Base values for indices 1..6. */
    static const seq_t base[7] = {
        0,                      /* unused 0‑index */
        (seq_t)8,               /* a(1) */
        (seq_t)119,             /* a(2) */
        (seq_t)1712,            /* a(3) */
        (seq_t)24246,           /* a(4) */
        (seq_t)342207,          /* a(5) */
        (seq_t)4823826          /* a(6) */
    };

    /* Expand cache if needed. */
    if ((size_t)n >= cache_sz) {
        size_t new_sz = (size_t)n + 1;                     /* indices 0..n */
        seq_t *new_cache = (seq_t *)malloc(new_sz * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failed */

        /* Copy old cached values (if any). */
        if (cache) {
            for (size_t i = 0; i < cache_sz; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }

        /* Ensure base values 1..6 are set. */
        for (size_t i = 1; i <= 6 && i < new_sz; ++i)
            new_cache[i] = base[i];

        /* Compute new values using the recurrence. */
        size_t start = cache_sz;
        if (start < 7)                /* haven't computed a(7) yet */
            start = 7;
        for (size_t i = start; i <= (size_t)n; ++i) {
            new_cache[i] = (seq_t)(
                21 * new_cache[i-1] -
                112 * new_cache[i-2] +
                217 * new_cache[i-3] -
                157 * new_cache[i-4] +
                36 * new_cache[i-5]);
        }

        cache   = new_cache;
        cache_sz = new_sz;
    }

    return cache[(size_t)n];
}
