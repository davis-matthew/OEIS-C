/*
 * A209822  Number of (n+1) X 2 0..2 arrays with every 2 X 2 subblock having
 *          one or two distinct values, and new values 0..2 introduced in row
 *          major order.
 * https://oeis.org/A209822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=8, a(2)=41, a(3)=218 Recurrence: a(n) = 6*a(n-1) - 3*a(n-2)
 *            - 2*a(n-3) for n >= 4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, free, abort */

static seq_t *cache = NULL;          /* array of cached terms */
static size_t cache_cap = 0;          /* allocated size of 'cache' */
static size_t cache_len = 0;          /* number of terms currently stored */

/* Ensure that 'cache' can hold at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap;
    if (new_cap == 0)
        new_cap = 4;                     /* start with capacity 4 (covers at least 3 init terms) */
    while (new_cap < need)
        new_cap <<= 1;                   /* double until large enough */

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                         /* out‑of‑memory */

    for (size_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];

    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Guard against calls with n == 0 – though outside the defined range. */
    if (n == 0)
        return (seq_t)0;

    /* First call: initialise cache with the initial three terms. */
    if (!cache) {
        ensure_cache_cap(4);              /* at least space for 4 terms */
        cache[0] = (seq_t)8;              /* a(1) */
        cache[1] = (seq_t)41;             /* a(2) */
        cache[2] = (seq_t)218;            /* a(3) */
        cache_len = 3;
    }

    /* If the requested value is already cached, return it directly. */
    if (n <= cache_len)
        return cache[(size_t)(n - 1)];

    /* Extend the cache up to the requested term. */
    ensure_cache_cap((size_t)n);

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Indices i-2, i-3, i-4 are valid because i >= 4 when this loop runs. */
        seq_t val = (seq_t)6 * cache[(size_t)(i - 2)] -
                    (seq_t)3 * cache[(size_t)(i - 3)] -
                    (seq_t)2 * cache[(size_t)(i - 4)];
        cache[(size_t)(i - 1)] = val;
    }

    cache_len = (size_t)n;
    return cache[(size_t)(n - 1)];
}
