/*
 * A137256  Binomial transform of 2^n, 2^n, 2^n.
 * https://oeis.org/A137256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3); a(0)=1, a(1)=2, a(2)=4
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cached values of the sequence; stored from a(0) up to a(cap-1) */
static seq_t *cache  = NULL;
static size_t  cap   = 0;

/* Ensure the cache can hold indices up to n; grows if necessary */
static void ensure_cap(idx_t n)
{
    size_t new_cap = (size_t)(n + 1);
    if (cap < new_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);               /* abort on allocation failure */
        cache = tmp;
        cap   = new_cap;
    }
}

/* seq_t a(idx_t n)  -- returns the n‑th term of A137256 */
seq_t a(idx_t n)
{
    /* initialise first three terms on first call */
    if (!cache) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 4;
        cap = 3;
    }

    /* if we already have the value, return it */
    if ((size_t)n < cap)                      /* cast to size_t for comparison */
        return cache[(size_t)n];

    /* compute any missing values up to n */
    size_t old_cap = cap;
    ensure_cap(n);                            /* guarantees indices up to n are valid */
    for (idx_t i = old_cap; i <= n; ++i) {
        size_t si   = (size_t)i;
        size_t s1   = si - 1;
        size_t s2   = si - 2;
        size_t s3   = si - 3;
        cache[si] = (seq_t)3 * (cache[s1] - cache[s2] + cache[s3]);
    }

    return cache[(size_t)n];
}
