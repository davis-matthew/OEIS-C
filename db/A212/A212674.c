/*
 * A212674  Number of (w,x,y,z) with all terms in {1,...,n} and w > |x-y| +
 *          |y-z|.
 * https://oeis.org/A212674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 5*a(n-4) - 4*a(n-5) + a(n-6), with
 *            a(0)=0, a(1)=1, a(2)=8, a(3)=35, a(4)=104, a(5)=247
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* pre‑computed initial values */
static const seq_t base_vals[6] = {0, 1, 8, 35, 104, 247};

/* memoisation cache */
static seq_t *cache   = NULL;  /* array of computed a(n)   */
static size_t cache_len = 0;   /* number of elements stored   */
static size_t cache_cap = 0;   /* allocated capacity          */

/* ensure the cache has room for indices 0..needed */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* initialise cache with the first six terms */
static void init_cache(size_t up_to)
{
    grow_cache(up_to + 1);
    memcpy(cache, base_vals, sizeof(base_vals));
    cache_len = 6;
}

/* make sure the cache contains a(k) for all k <= n */
static void ensure_cache(idx_t n)
{
    size_t up_to = (size_t)n;

    if (up_to < cache_len)
        return;                     /* already available */

    if (!cache)
        init_cache(up_to + 1);       /* first allocation */
    else if (up_to >= cache_cap)
        grow_cache(up_to + 1);

    /* compute missing terms using the recurrence */
    for (size_t i = cache_len; i <= up_to; ++i)
    {
        seq_t v1 = cache[i - 1];
        seq_t v2 = cache[i - 2];
        seq_t v4 = cache[i - 4];
        seq_t v5 = cache[i - 5];
        seq_t v6 = cache[i - 6];

        seq_t val = v1 * 4;          /* 4*a(n-1)   */
        val += v4 * 5;              /* +5*a(n-4)  */
        val += v6;                  /* +   a(n-6) */
        val -= v2 * 5;              /* -5*a(n-2)  */
        val -= v5 * 4;              /* -4*a(n-5)  */
        cache[i] = val;              /* store result */
    }
    cache_len = up_to + 1;
}

seq_t a(idx_t n)
{
    if (n < 6)
        return base_vals[(size_t)n];

    ensure_cache(n);
    return cache[(size_t)n];
}
