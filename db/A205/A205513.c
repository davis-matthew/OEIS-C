/*
 * A205513  Number of (n+1) X 2 0..2 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock differing from each horizontal
 *          or vertical neighbor.
 * https://oeis.org/A205513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*a(n-2) + 6*a(n-3) + 39*a(n-4) - 2*a(n-5) - 3*a(n-6) -
 *            4*a(n-7) Initial values: a(1)=81, a(2)=360, a(3)=1683,
 *            a(4)=7572, a(5)=35577, a(6)=160218, a(7)=750426
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cached sequence values, 1‑based */
static size_t cache_cap = 0;    /* capacity of the cache (max index storable) */
static idx_t  cache_max = 0;    /* largest index already computed */

/* grows the cache to hold indices up to n */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 8;
    while ((size_t)n > new_cap)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    if (!new_cache) exit(1);          /* terminate on allocation failure */

    for (size_t i = 1; i <= cache_max; ++i)
        new_cache[i] = cache[i];

    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise the cache with known values */
    if (!cache) {
        cache = (seq_t *)malloc(9 * sizeof(seq_t)); /* indices 1..8 (0 unused) */
        if (!cache) exit(1);
        cache_cap = 8;
        cache[1] = 81;
        cache[2] = 360;
        cache[3] = 1683;
        cache[4] = 7572;
        cache[5] = 35577;
        cache[6] = 160218;
        cache[7] = 750426;
        cache_max = 7;
    }

    if (n <= cache_max)
        return cache[n];

    ensure_capacity(n);

    /* compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 7)     /* base values already set */
            continue;

        /* a(i) = 18*a(i-2) + 6*a(i-3) + 39*a(i-4)
                 - 2*a(i-5) - 3*a(i-6) - 4*a(i-7) */
        seq_t pos = 18 * cache[i-2] + 6 * cache[i-3] + 39 * cache[i-4];
        seq_t sub = 2 * cache[i-5] + 3 * cache[i-6] + 4 * cache[i-7];
        cache[i] = pos - sub;
    }

    cache_max = n;
    return cache[n];
}
