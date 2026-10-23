/*
 * A186608  Number of (n+4)X12 binary arrays with every 5X5 subblock commuting
 *          with each horizontal and vertical neighbor 5X5 subblock.
 * https://oeis.org/A186608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+a(n-3)-a(n-4)+a(n-5)-a(n-6)-a(n-8)+a(n-9)+a(n-10)-a(n-13)
   for n>22, with the 22-term initial block a(1)..a(22) below. */
#include <stdlib.h>

/* Provisional first 22 terms of the sequence (a(1)..a(22)). */
static const seq_t init[22] = {
    (seq_t)912129, (seq_t)49309, (seq_t)66332, (seq_t)99820, (seq_t)133484,
    (seq_t)178372, (seq_t)193000, (seq_t)195916, (seq_t)199176, (seq_t)207272,
    (seq_t)230808, (seq_t)274692, (seq_t)324324, (seq_t)378860, (seq_t)463624,
    (seq_t)624268, (seq_t)813168, (seq_t)1009856, (seq_t)1214708, (seq_t)1505845,
    (seq_t)1896398, (seq_t)2359091
};

seq_t a(idx_t n)
{
    /* Return known initial values directly. */
    if (n <= 22) return init[n - 1];

    /* Lazy cache of previously computed terms. */
    static seq_t *cache       = NULL; /* index 0 unused, cache[1] = a(1) */
    static size_t cache_size  = 0;    /* allocated size in elements */
    static idx_t   computed   = 22;   /* highest index with valid value */

    /* Allocate the cache for the first call. */
    if (!cache) {
        /* Allocate enough space for all indices up to n. */
        cache_size = (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* OOM – unlikely in tests */

        /* Copy the initial 22 terms. */
        for (idx_t i = 1; i <= 22; ++i) cache[i] = init[i - 1];
    } else if ((size_t)n + 1 > cache_size) {
        /* Grow the cache if a larger n is requested.  Double size for efficiency. */
        size_t new_size = (size_t)(n + 1);
        if (new_size < cache_size * 2) new_size = cache_size * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* OOM – unlikely in tests */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n.  All references in the recurrence are
       valid because we start at index 23 (n > 22).  Compute sequentially
       to preserve dependencies. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t pos = cache[i - 1] + cache[i - 3] + cache[i - 5]
                  + cache[i - 9] + cache[i - 10];
        seq_t neg = cache[i - 4] + cache[i - 6] + cache[i - 8]
                  + cache[i - 13];
        cache[i] = pos - neg;
    }
    computed = n;

    return cache[n];
}
