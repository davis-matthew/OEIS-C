/*
 * A178901  a(1)=2; for n > 1, a(n) is the largest number <= 2*a(n-1)
 *          divisible by n.
 * https://oeis.org/A178901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 2; for n > 1,
   a(n) = 2*a(n-1) - (2*a(n-1)) % n  (largest multiple of n ≤ 2*a(n-1)) */

#include <stdlib.h>      /* for realloc, abort */
#include <stddef.h>      /* for size_t */

/* Static cache for memoisation (a(1) → cache[0]) */
static seq_t *cache   = NULL;   /* pointer to the cached terms */
static size_t cache_cap = 0;    /* allocated number of elements */
static size_t cache_len = 0;    /* highest term that has been computed */

/* Ensure the cache can hold at least `idx` elements (1‑based). */
static void ensure_cache(idx_t idx)
{
    size_t needed = (size_t)idx;
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* out of memory – terminate safely */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence, where n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n < 1, return 0 sentinel */

    ensure_cache(n);

    /* Compute any terms that have not yet been cached. */
    if (cache_len < (size_t)n)
    {
        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            if (i == 1)
            {
                cache[0] = (seq_t)2;          /* a(1) = 2 */
            }
            else
            {
                seq_t prev = cache[i - 2];    /* a(i-1) */
                seq_t max_val = prev * (seq_t)2;
                seq_t rem = max_val % (seq_t)i;
                cache[i - 1] = max_val - rem; /* largest multiple of i ≤ 2*a(i-1) */
            }
        }
        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
