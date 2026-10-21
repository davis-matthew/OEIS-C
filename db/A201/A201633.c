/*
 * A201633  Numbers k such that Sum_{j=0..3} (k + j)^2 is a triangular number.
 * https://oeis.org/A201633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A201633
 *  Recurrence (indices n ≥ 1):
 *         a(n+1) = a(n) + 34·a(n-1) – 34·a(n-2) – a(n-3) + a(n-4)
 *  Initial values (a(1) .. a(5)):
 *         11, 28, 424, 1001, 14453
 *  The function memoises computed terms so that subsequent requests for
 *  any n are answered in O(1) after the first expansion.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *cache = NULL;      /* cache[i] holds a(i+1) */
static size_t cache_used = 0;    /* number of stored elements */
static size_t cache_cap  = 0;    /* allocated capacity */

/* Ensure the cache has at least `need` capacity */
static void grow_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap < 8 ? 8 : cache_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)
        abort();                /* allocation failed – signal error */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the first five terms of the sequence */
static void init_cache(void)
{
    grow_cache(5);
    cache[0] = (seq_t)11;      /* a(1) */
    cache[1] = (seq_t)28;      /* a(2) */
    cache[2] = (seq_t)424;     /* a(3) */
    cache[3] = (seq_t)1001;    /* a(4) */
    cache[4] = (seq_t)14453;   /* a(5) */
    cache_used = 5;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined; return 0 as placeholder */
        return (seq_t)0;

    if (cache_used == 0)       /* first call – populate initial terms */
        init_cache();

    /* Return a pre‑computed term */
    if ((size_t)(n - 1) < cache_used)
        return cache[n - 1];

    /* Expand cache to include up to a(n) */
    size_t target = (size_t)n;            /* number of terms needed */
    grow_cache(target);

    /* Compute missing terms using the recurrence */
    for (size_t i = cache_used; i < target; ++i)
    {
        /* a_{i+1} = a_i + 34*a_{i-1} – 34*a_{i-2} – a_{i-3} + a_{i-4} */
        cache[i] = cache[i - 1]
                   + (seq_t)34 * cache[i - 2]
                   - (seq_t)34 * cache[i - 3]
                   - cache[i - 4]
                   + cache[i - 5];
    }

    cache_used = target;
    return cache[n - 1];
}
