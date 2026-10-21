/*
 * A218723  a(n) = (256^n - 1)/255.
 * https://oeis.org/A218723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (256^n - 1)/255 * Recurrence (used for the fast linear
 *            implementation): * a(0) = 0, a(1) = 1 * a(n) = 257*a(n-1) -
 *            256*a(n-2) (n >= 2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *cache = NULL;      /* cache of computed terms          */
static idx_t cache_len = 0;      /* number of elements in cache     */
static idx_t cache_cap = 0;      /* allocated size of cache         */

/* Ensure that the cache can hold at least 'need' elements.  */
static void ensure_cache(idx_t need)
{
    if (cache_cap >= need)
        return;
    idx_t new_cap = cache_cap == 0 ? 1 : cache_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In a production program we would handle the error
           properly; abort is sufficient for the kata.            */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/*  Compute the nth term of A218723.  */
seq_t a(idx_t n)
{
    /* Handle the first term explicitly.   */
    if (n == 0) {
        if (cache_len < 1) {
            ensure_cache(1);
            cache[0] = (seq_t)0;
            cache_len = 1;
        }
        return cache[0];
    }

    /* Make sure we have space for indices up to n. */
    if (cache_cap < n + 1)
        ensure_cache(n + 1);

    /* Fill missing terms using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)0;
        else if (i == 1)
            cache[i] = (seq_t)1;
        else
            cache[i] = cache[i-1] * (seq_t)257 - cache[i-2] * (seq_t)256;
    }

    cache_len = n + 1;
    return cache[n];
}
