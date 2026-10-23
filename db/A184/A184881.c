/*
 * A184881  a(n) = A184879(2*n, n) - A184879(2*n, n+1) where A184879(n, k) =
 *          Hypergeometric2F1(-2*k, 2*k-2*n, 1, -1) if 0<=k<=n.
 * https://oeis.org/A184881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   * a(n) = A184879(2*n, n) - A184879(2*n, n+1) * Recurrence for n
 *            >= 2: * a(n) = -2*(2*n-3)/(n+1) * a(n-1) * Initial values: *
 *            a(0) = 1 * a(1) = -3 * * Memoized implementation suitable for
 *            arbitrary order of calls
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 18, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoization array */
static idx_t cache_len = 0;   /* number of elements currently stored */

/* Ensure cache contains values up to index n (inclusive). */
static void
ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;                       /* already computed */

    /* Allocate or extend the cache. */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        abort();                      /* out of memory */
    cache = tmp;

    /* Initialise the base cases if necessary. */
    if (cache_len == 0) {              /* first time */
        cache[0] = (seq_t)1;
        if (new_len > 1)
            cache[1] = (seq_t)-3;
    } else if (cache_len == 1 && new_len > 1) { /* grow from 1 to 2 */
        cache[1] = (seq_t)-3;
    }

    /* Compute missing values using the recurrence. */
    for (idx_t i = (cache_len > 2 ? cache_len : 2);
         i <= n;
         ++i)
    {
        /* m = -2*(2*i-3)   (computed in seq_t to avoid overflow) */
        seq_t m = -((seq_t)2 * (((seq_t)2 * (seq_t)i) - (seq_t)3));
        /* denom = i + 1 */
        seq_t denom = (seq_t)(i + 1);
        cache[i] = (m * cache[i - 1]) / denom;
    }

    cache_len = new_len;
}

seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
