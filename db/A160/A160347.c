/*
 * A160347  Numerator of Hermite(n, 29/31).
 * https://oeis.org/A160347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 58, a(n) = 58*a(n-1) - 2*(n-1)*961*a(n-2) for n
 *            >= 2 (Numerator of Hermite(n, 29/31) evaluated at x = 29/31)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for memoization */
static seq_t *cache   = NULL;  /* array of previously computed terms */
static size_t cache_cap = 0;   /* allocated capacity of cache   */
static size_t cache_len = 0;   /* number of terms actually stored */

/* Ensure that the cache can hold indices up to 'need'. */
static void ensure_capacity(size_t need)
{
    if (need < cache_cap)
        return;

    size_t new_cap = cache_cap ? (cache_cap << 1) : 2;
    while (new_cap <= need)
        new_cap <<= 1;

    /* Allocate new array and copy old values */
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();                  /* out‑of‑memory */

    for (size_t i = 0; i < cache_len; ++i)
        tmp[i] = cache[i];

    if (cache != NULL)
        free(cache);

    cache     = tmp;
    cache_cap = new_cap;
}

/* Public function computing the n-th term. */
seq_t a(idx_t n)
{
    /* Invariant: cache[0] = 1, cache[1] = 58 when cache_len >= 2. */

    /* Initialise cache on first call */
    if (cache_len == 0) {
        ensure_capacity(1);              /* capacity for indices 0 and 1 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)58;
        cache_len = (size_t)2;
    }

    size_t idx = (size_t)n;

    /* Extend cache to contain index n */
    ensure_capacity(idx);

    /* Compute missing terms if any */
    for (size_t i = cache_len; i <= idx; ++i) {
        /* i == 0 or 1 already initialised */
        if (i < 2)
            continue;

        /* a(i) = 58*a(i-1) - 2*(i-1)*961*a(i-2) */
        seq_t term1 = (seq_t)58 * cache[i - 1];

        /* multiplier = 2*(i-1)*961 */
        unsigned long long mult_small = 2ULL * (unsigned long long)(i - 1) * 961ULL;
        seq_t term2 = cache[i - 2] * (seq_t)mult_small;

        cache[i] = term1 - term2;
    }

    /* Update number of stored terms */
    if (idx + 1 > cache_len)
        cache_len = idx + 1;

    return cache[idx];
}
