/*
 * A173096  a(1) = 3, a(2) = 4, a(n) = a(n-2)*a(n-1)-a(n-2)-a(n-1).
 * https://oeis.org/A173096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2)*a(n-1) - a(n-2) - a(n-1); a(1)=3, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc, exit */

static seq_t *cache = NULL;        /* 1‑based cache of computed terms */
static size_t cap = 0;             /* allocated size of cache (+1 for index 0) */
static idx_t maxComputed = 0;      /* largest n that has been computed */

/* Ensure the cache can hold indices up to 'needed' (inclusive). */
static void ensure_capacity(size_t needed)
{
    if (needed <= cap)
        return;
    size_t newcap = cap ? cap * 2 : 4;
    if (newcap < needed)
        newcap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp)
        exit(1);            /* out of memory – terminate program */
    cache = tmp;
    cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for OEIS offset 1, but safe fallback */

    /* Initialize cache on first call. */
    if (maxComputed == 0) {
        ensure_capacity(3);   /* need indices 0..2 */
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)4;
        maxComputed = 2;
    }

    /* Compute any missing terms up to n. */
    if (n > maxComputed) {
        /* Ensure array large enough. */
        ensure_capacity((size_t)n + 1);
        for (idx_t i = maxComputed + 1; i <= n; ++i) {
            cache[i] = cache[i-2] * cache[i-1] - cache[i-2] - cache[i-1];
        }
        maxComputed = n;
    }

    return cache[n];
}
