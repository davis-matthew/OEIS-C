/*
 * A235287  Number of (n+1) X (6+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - 15*a(n-3) + 4*a(n-4) + 18*a(n-5) -
 *            12*a(n-6), with a(1)=472, a(2)=572, a(3)=680, a(4)=908,
 *            a(5)=1168, a(6)=1724
 * Range:     u8 none, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation. */
static seq_t *cache = NULL;     /* array index is n (1‑based) */
static idx_t  cache_size = 0;   /* highest n that has been computed */
static idx_t  cache_cap   = 0;   /* allocated capacity (>= cache_size+1) */

/* Ensure that the cache has capacity at least (n + 1). */
static void ensure_cache(idx_t n)
{
    if (cache_cap < n + 1)
    {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* out of memory – abort */
        cache      = tmp;
        cache_cap  = new_cap;
    }
}

/* Return a(n) for n ≥ 1.  Uses memoisation; works in any order of calls. */
seq_t a(idx_t n)
{
    if (n <= 6)
    {
        /* Base values – no allocation required. */
        static const seq_t base[6] = { 472, 572, 680, 908, 1168, 1724 };
        return base[n - 1];
    }

    /* Ensure we have enough space in the cache. */
    ensure_cache(n);

    /* Initialise the first six terms if the cache is empty or new. */
    if (cache_size < 6)
    {
        cache[1] = (seq_t)472;
        cache[2] = (seq_t)572;
        cache[3] = (seq_t)680;
        cache[4] = (seq_t)908;
        cache[5] = (seq_t)1168;
        cache[6] = (seq_t)1724;
        cache_size = 6;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];

        /* Positive part of the recurrence. */
        seq_t pos = (seq_t)3 * a1 + (seq_t)3 * a2 + (seq_t)4 * a4 + (seq_t)18 * a5;
        /* Negative part of the recurrence. */
        seq_t neg = (seq_t)15 * a3 + (seq_t)12 * a6;

        cache[i] = pos - neg;   /* safe: pos ≥ neg for all n */
    }

    cache_size = n;
    return cache[n];
}
