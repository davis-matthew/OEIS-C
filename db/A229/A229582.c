/*
 * A229582  Number of defective 3-colorings of an n X 4 0..2 array connected
 *          horizontally and antidiagonally with exactly one mistake, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-1) - 81*a(n-2) for n > 3 initial values: a(1)=6,
 *            a(2)=116, a(3)=1620
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* memoisation array                     */
static size_t cache_cap = 0;       /* current capacity of the cache          */
static size_t cached_len = 0;      /* number of terms stored (a(1)..a(len)) */

/* Ensure the cache can hold at least 'needed' elements. */
static void _ensure_cache(size_t needed)
{
    if (cache_cap < needed)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Assume n >= 1 (offset 1). */
    if (cached_len == 0)
    {
        /* initialise the first three known terms */
        _ensure_cache(3);
        cache[0] = (seq_t)6;     /* a(1) */
        cache[1] = (seq_t)116;   /* a(2) */
        cache[2] = (seq_t)1620;  /* a(3) */
        cached_len = 3;
    }

    if ((size_t)n <= cached_len)
        return cache[(size_t)(n - 1)];   /* already known */

    /* extend the memoisation array up to n using the recurrence   */
    for (idx_t i = cached_len + 1; i <= n; ++i)
    {
        _ensure_cache((size_t)i);
        /* a(i) = 18*a(i-1) - 81*a(i-2)  for i > 3  */
        seq_t val = (seq_t)18 * cache[(size_t)(i - 2)]    /* a(i-1)   */
                  - (seq_t)81 * cache[(size_t)(i - 3)];  /* a(i-2)   */
        cache[(size_t)(i - 1)] = val;                    /* store a(i) */
    }
    cached_len = (size_t)n;
    return cache[(size_t)(n - 1)];
}
