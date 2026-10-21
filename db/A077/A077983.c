/*
 * A077983  Expansion of 1/(1 + 2*x - 2*x^2 + x^3).
 * https://oeis.org/A077983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) + 2*a(n-2) - a(n-3) with a(0)=1, a(1)=-2,
 *            a(2)=6
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 20, i64 n <= 42, i128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoization. */
static seq_t *cache      = NULL; /* allocated array of terms */
static size_t cache_len  = 0;    /* number of terms currently stored (0..n) */
static size_t cache_cap  = 0;    /* capacity of cache */

/* Ensure the cache has capacity for at least n+1 terms. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return; /* already enough */

    /* grow to at least required size, doubling strategy */
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap <= (size_t)n)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure: abort, the program cannot continue anyway */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the first three terms if they are not yet stored. */
static void initialise_base(void)
{
    if (cache_len >= 3)
        return;

    ensure_capacity(2); /* at least 3 slots */

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)-2;
    cache[2] = (seq_t)6;
    cache_len = 3;
}

/* Compute a(n) using memoised recurrence. */
seq_t a(idx_t n)
{
    /* Normalise index: 0-based already. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Initialise base terms if needed. */
    initialise_base();

    /* Ensure capacity for all terms up to n. */
    ensure_capacity(n);

    /* Compute missing terms up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)-2;
        } else if (i == 2) {
            cache[i] = (seq_t)6;
        } else {
            /* recurrence: a(n) = -2*a(n-1) + 2*a(n-2) - a(n-3) */
            cache[i] = (seq_t)-2 * cache[i-1] + (seq_t)2 * cache[i-2] - cache[i-3];
        }
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
