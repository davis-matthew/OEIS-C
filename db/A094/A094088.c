/*
 * A094088  E.g.f. 1/(2-cosh(x)) (even coefficients).
 * https://oeis.org/A094088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; for n ≥ 1
   a(n) = ∑_{k=1..n} C(2n, 2k) · a(n‑k) */
#include <stddef.h>
#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build system.
   seq_t may be an unsigned integer or an arbitrary‑precision class. */

/* Compute a(n).  Results for requested indices are memoized. */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cache[0..cache_len‑1] = a(0)…a(n) */
    static size_t cache_len = 0;    /* number of cached elements */

    /* Bootstrap cache with a(0) = 1 on first call. */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */
        cache[0] = 1;
        cache_len = 1;
    }

    /* If the requested n is already cached, return it. */
    if ((idx_t)(cache_len - 1) >= n)
        return cache[(size_t)n];

    /* Grow the cache to hold indices up to n. */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;                /* allocation failure */
    cache = tmp;
    size_t cur = cache_len;            /* index of next element to compute */

    /* Compute missing terms up to n. */
    for (; cur <= (size_t)n; ++cur) {
        idx_t ci = cur;                /* current index (as idx_t) */

        /* a(0) is already 1, skip its computation. */
        if (ci == 0) {
            cache[0] = 1;
            continue;
        }

        seq_t sum = 0;

        /* 2n as seq_t */
        seq_t two_n = (seq_t)2 * (seq_t)ci;

        /* C(2n, 2) */
        seq_t binom = (two_n * (two_n - 1)) / 2;

        /* k = 1 term */
        sum += binom * cache[ci - 1];

        /* Remaining k ≥ 2 */
        for (idx_t k = 2; k <= ci; ++k) {
            /* Compute ratio to update binom:
             * C(2n,2k) = C(2n,2k-2) * (2n-2k+2)(2n-2k+1) / ((2k)(2k-1)) */
            seq_t t1 = two_n - (seq_t)2 * (k - 1);
            seq_t t2 = t1 - 1;
            seq_t num = t1 * t2;                /* (2n-2k+2)(2n-2k+1) */
            seq_t den = (seq_t)2 * k * ((seq_t)2 * k - 1); /* (2k)(2k-1) */
            binom = binom * num / den;            /* exact division */

            sum += binom * cache[ci - k];
        }

        cache[(size_t)ci] = sum;
    }

    cache_len = new_len;
    return cache[(size_t)n];
}
