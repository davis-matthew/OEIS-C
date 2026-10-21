/*
 * A234452  Number of (n+1) X (1+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 100*a(n-2) - 1384*a(n-4) + 1440*a(n-6) for n≥7 Initial
 *            values: a(1)=280, a(2)=2400, a(3)=21344, a(4)=192960,
 *            a(5)=1755520, a(6)=16026240. a(0) is unused
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache for memoization */
static seq_t *cache = NULL;
static idx_t cache_len = 0;      /* highest index computed so far */
static size_t cache_cap = 0;     /* allocated capacity (elements) */

static void ensure_cache(idx_t n)
/* Expand cache to at least “n” elements, computing values by
   the recurrence when needed. */
{
    if (n <= cache_len)            /* already have all needed values */
        return;

    /* Ensure we have capacity for indices 0..n */
    size_t needed = (size_t)n + 1; /* index 0 unused */
    if (cache == NULL || cache_cap < needed) {
        size_t newcap = cache_cap > 0 ? cache_cap * 2 : 8;
        if (newcap < needed)
            newcap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL)          /* allocation failed – abort */
            abort();
        cache = tmp;
        cache_cap = newcap;
    }

    /* Initialise the first six terms only once */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;                /* unused */
        cache[1] = (seq_t)280;
        cache[2] = (seq_t)2400;
        cache[3] = (seq_t)21344;
        cache[4] = (seq_t)192960;
        cache[5] = (seq_t)1755520;
        cache[6] = (seq_t)16026240;
        cache_len = 6;
    }

    /* Compute missing terms via the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* i≥7 here because cache_len starts at 6          */
        seq_t term = cache[i - 2] * (seq_t)100;
        term -= cache[i - 4] * (seq_t)1384;
        term += cache[i - 6] * (seq_t)1440;
        cache[i] = term;
    }

    if (n > cache_len)
        cache_len = n;
}

seq_t a(idx_t n)
/* Return a(n) for n≥1.  For n==0 return 0 (unused offset). */
{
    if (n == 0)
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
