/*
 * A041256  Numerators of continued fraction convergents to sqrt(140).
 * https://oeis.org/A041256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 142*a(n-4) - a(n-8) for n ≥ 8, with a(0)=11, a(1)=12,
 *            a(2)=59, a(3)=71, a(4)=1621, a(5)=1692, a(6)=8389, a(7)=10081
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache for memoization */
static seq_t *cache = NULL;
static size_t cache_len = 0;     /* highest index computed + 1 */
static size_t cache_cap = 0;     /* allocated capacity */
static bool cache_initialized = false;

/* ensure cache has space for at least 'needed' elements */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* initialise the first eight values */
static void init_cache(void)
{
    ensure_cache_capacity(8);
    cache[0] = (seq_t)11;
    cache[1] = (seq_t)12;
    cache[2] = (seq_t)59;
    cache[3] = (seq_t)71;
    cache[4] = (seq_t)1621;
    cache[5] = (seq_t)1692;
    cache[6] = (seq_t)8389;
    cache[7] = (seq_t)10081;
    cache_len = 8;
    cache_initialized = true;
}

seq_t a(idx_t n)
{
    size_t ui = (size_t)n;      /* use size_t for array indexing */

    /* initialise cache on first call */
    if (!cache_initialized)
        init_cache();

    /* if n is within the initial block, return directly */
    if (ui < 8)
        return cache[ui];

    /* ensure we have room for n+1 entries */
    ensure_cache_capacity(ui + 1);

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= ui; ++i) {
        /* recurrence: a(i) = 142*a(i-4) - a(i-8) */
        cache[i] = (seq_t)142 * cache[i - 4] - cache[i - 8];
    }
    cache_len = ui + 1;

    return cache[ui];
}
