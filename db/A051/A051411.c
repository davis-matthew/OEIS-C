/*
 * A051411  a(n+1) = a(n) + sum of digits of a(n)^3.
 * https://oeis.org/A051411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 47, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A051411
 * a(1) = 7
 * a(n+1) = a(n) + sum of decimal digits of a(n)^3   for all n >= 1
 * This implementation uses a static cache to allow any order of
 * calls and achieves O(n) time per call (worst‑case for unseen terms).
 *
 * Note: seq_t and idx_t are defined by the build; seq_t may be a
 * 8, 16, 32, 64‑bit unsigned integer or an arbitrary‑precision integer class.
 * Only integer arithmetic, comparisons, and bitwise operators are used.
 */

#include <stdlib.h>   /* for realloc */

/* Compute the sum of decimal digits of a seq_t value.   */
static inline seq_t digit_sum_seq(seq_t v)
{
    seq_t sum = 0;
    while (v > 0) {
        sum += v % 10;
        v   /= 10;
    }
    return sum;
}

/* Sequence entry for index n (n >= 1). */
seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i+1) for i >= 0. */
    static seq_t *cache      = NULL;   /* pointer to allocated array */
    static size_t cache_cap  = 0;      /* allocated capacity (in terms) */
    static size_t cache_len  = 0;      /* already computed terms */

    /* Sanity check: if n is larger than SIZE_MAX we cannot store it in
     * size_t, so we handle it by realloc with size_t bound. */
    if (n > (idx_t)SIZE_MAX) {
        /* n too large to cache; compute directly by iterating up to n,
         * but no cache expansion. */
        /* As n is expected to be small in typical usage, this scenario
         * is unlikely. */
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            /* Compute on the fly without caching */
            seq_t prev, cube, ds;
            if (i == 0)
                prev = 7;
            else
                prev = cache[i - 1];
            cube = prev * prev * prev;
            ds = digit_sum_seq(cube);
            if (i == 0)
                cache[i] = 7 + ds;
            else
                cache[i] = prev + ds;
        }
        return cache[(size_t)n - 1];
    }

    /* Ensure the cache has enough capacity. */
    size_t required = (size_t)n;
    if (required > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      required * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed; in the test harness this should not
             * happen for the expected ranges.  Return 0 as a simple
             * error value. */
            return 0;
        }
        cache      = tmp;
        cache_cap  = required;
    }

    /* Compute missing terms up to a(n). */
    if ((size_t)n > cache_len) {
        /* Base case: a(1) = 7 if no terms computed yet. */
        if (cache_len == 0) {
            cache[0] = 7;
            cache_len = 1;
        }

        for (size_t i = cache_len; i < (size_t)n; ++i) {
            seq_t prev  = cache[i - 1];             /* a(i)     */
            seq_t cube  = prev * prev * prev;       /* a(i)^3   */
            seq_t ds    = digit_sum_seq(cube);      /* sum digits */
            cache[i]    = prev + ds;                /* a(i+1)   */
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
