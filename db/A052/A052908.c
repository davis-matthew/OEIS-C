/*
 * A052908  Expansion of 1 + x/(1 - 2*x - x^3 + x^4).
 * https://oeis.org/A052908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-3) - a(n-4) for n ≥ 5 Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=9
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 60, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;         /* cached values a(0)…a(k) */
static size_t  cache_len = 0;       /* number of filled entries            */
static size_t  cache_cap = 0;       /* allocated capacity                   */

/* Ensure that cache has room for at least `min_cap` elements. */
static void ensure_capacity(size_t min_cap)
{
    if (min_cap <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < min_cap)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Initialise cache with the first five terms if it is empty. */
static void init_cache(void)
{
    if (cache_len > 0)
        return;
    ensure_capacity(5);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)4;
    cache[4] = (seq_t)9;
    cache_len = 5;
}

seq_t a(idx_t n)
{
    if (n < 5)
    {
        init_cache();
        return cache[(size_t)n];
    }

    init_cache();

    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    size_t target = (size_t)n + 1;
    ensure_capacity(target);

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        /* recurrence: a[i] = 2*a[i-1] + a[i-3] - a[i-4]  (i ≥ 5) */
        seq_t term = cache[i - 1] * 2 + cache[i - 3];
        term = term - cache[i - 4];
        cache[i] = term;
    }

    cache_len = target;
    return cache[(size_t)n];
}
