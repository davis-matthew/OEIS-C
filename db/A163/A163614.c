/*
 * A163614  a(n) = ((1 + 3*sqrt(2))*(3 + sqrt(2))^n + (1 - 3*sqrt(2))*(3 -
 *          sqrt(2))^n)/2.
 * https://oeis.org/A163614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1)-7*a(n-2), a(0)=1, a(1)=9
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Lazy cache for the recurrence.  The array is grown on demand. */
static seq_t *cache      = NULL;   /* pointer to cached terms          */
static size_t cache_cap  = 0;       /* allocated number of entries      */
static size_t cache_used = 0;       /* number of entries that are set   */

/* Ensure that the cache can hold indices up to n (inclusive).  New
 * entries are left uninitialised.  Existing entries are preserved. */
static void
ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;

    /* Grow the allocated size (at least double, or enough for n). */
    size_t new_cap = (cache_cap == 0) ? 4 : cache_cap << 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    /* Allocate the new array.  sC++-style cast is required for C++. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        exit(1);          /* unrecoverable memory allocation failure */

    cache      = tmp;
    cache_cap  = new_cap;

    /* Initialise the first two terms once, after the first allocation. */
    if (cache_used == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache_used = 2;
    }
}

/* Main entry point: compute the n‑th term of A163614. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)9;

    ensure_cache(n);

    for (idx_t i = cache_used; i <= n; ++i)
        cache[i] = (seq_t)6 * cache[i - 1] - (seq_t)7 * cache[i - 2];

    cache_used = (size_t)n + 1;
    return cache[n];
}
