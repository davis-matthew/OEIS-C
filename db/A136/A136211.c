/*
 * A136211  Denominators in continued fraction [0; 1, 3, 1, 3, 1, 3, ...].
 * https://oeis.org/A136211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(1)=1, a(2)=4; for n>2 if n is odd: a(n)=a(n-1)+a(n-2) if n is
 *            even: a(n)=3*a(n-1)+a(n-2)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 57, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based array of a(n) */
static size_t cache_cap = 0;  /* number of allocated entries */

/* Ensure that the cache can hold indices up to n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= (size_t)n)
        return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                      /* memory exhausted */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined for n<1 */

    /* Make sure the cache can hold up to n. */
    ensure_cache(n + 1);                /* +1 so that indices 1..n are valid */

    /* Initialize the first two terms (idempotent). */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)4;

    if (n <= 2)
        return cache[n];

    for (idx_t i = 3; i <= n; ++i)
    {
        if (i & 1)    /* odd index */
            cache[i] = cache[i - 1] + cache[i - 2];
        else          /* even index */
            cache[i] = cache[i - 1] * (seq_t)3 + cache[i - 2];
    }

    return cache[n];
}
