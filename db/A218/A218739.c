/*
 * A218739  a(n) = (36^n - 1)/35.
 * https://oeis.org/A218739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (36^n - 1)/35 = 36*a(n-1) + 1, a(0)=0, a(1)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Static cache for memoised values */
static seq_t *cache = NULL;      /* cache storage */
static size_t cache_cap = 0;     /* allocated capacity (number of seq_t slots) */
static size_t cache_len = 0;     /* number of values actually stored (0 .. cache_len-1) */

/* Ensure the cache can hold at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise base values a(0) = 0, a(1) = 1 */
static void init_cache(void)
{
    ensure_capacity(2);
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)1;
    cache_len = 2;
}

/* Return a(n) with memoisation and recurrence */
seq_t a(idx_t n)
{
    /* First call: set up base values */
    if (cache == NULL)
        init_cache();

    /* If already computed, just return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Grow cache if necessary */
    if ((size_t)n >= cache_cap)
        ensure_capacity((size_t)n + 1);

    /* Compute missing entries up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
        cache[i] = 36 * cache[i - 1] + 1;

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
