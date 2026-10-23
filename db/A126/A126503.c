/*
 * A126503  Number of base 8 n-digit numbers with adjacent digits differing by
 *          four or less.
 * https://oeis.org/A126503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 6*a(n-1) + 5*a(n-2) - 3*a(n-3) - 2*a(n-4) (for n > 3),
 *            with initial values a(0)=1, a(1)=8, a(2)=52, a(3)=348
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper array holding the first four terms */
static const seq_t init_seq[4] = {
    (seq_t)1,
    (seq_t)8,
    (seq_t)52,
    (seq_t)348
};

/* Compute a(n) with memoisation, independent of call order */
seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache      = NULL;
    static idx_t cached_last = 0;   /* highest n whose value is cached */
    static size_t cache_cap  = 0;   /* allocated capacity of cache */

    /* Direct return for the first four terms */
    if (n < 4)
        return init_seq[n];

    /* Initialise the cache on first use */
    if (cache == NULL) {
        cache_cap   = 8;                     /* initial capacity */
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;         /* allocation failure unlikely in tests */
        /* store the first four values */
        cache[0] = init_seq[0];
        cache[1] = init_seq[1];
        cache[2] = init_seq[2];
        cache[3] = init_seq[3];
        cached_last = 3;
    }

    /* Expand cache storage if needed */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;           /* allocation failure unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values, bottom‑up */
    for (idx_t i = cached_last + 1; i <= n; ++i) {
        seq_t val = (6 * cache[i-1]) + (5 * cache[i-2]) - (3 * cache[i-3]) - (2 * cache[i-4]);
        cache[i] = val;
    }
    cached_last = n;

    return cache[n];
}
