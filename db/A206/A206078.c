/*
 * A206078  Number of (n+1) X 2 0..3 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock equal to one.
 * https://oeis.org/A206078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 13*a(n-2) - 4*a(n-3) + 2*a(n-4) for n>4 with
 *            a(1)=124, a(2)=960, a(3)=7308, a(4)=55864
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* array of cached values, 1‑based */
static size_t cache_cap = 0;      /* allocated capacity (size_t) */
static idx_t  max_cached = 0;     /* largest n for which cache[i] is valid */

/* Helper to extend the cache to at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);     /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) with memoization.  The function is thread‑unsafe but
   satisfies the requirement of static linkage and correct results for
   any access order. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;         /* undefined below the offset */

    /* Initialise cache if needed. */
    if (cache == NULL) {
        size_t init_cap = (size_t)n + 1;
        if (init_cap < 5) init_cap = 5;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);
        cache_cap = init_cap;
        /* fill known initial values; indices 1..4 are set */
        cache[1] = (seq_t)124;
        cache[2] = (seq_t)960;
        cache[3] = (seq_t)7308;
        cache[4] = (seq_t)55864;
        max_cached = 4;
    }

    /* Ensure enough space for requested index. */
    if ((size_t)n + 1 > cache_cap)
        grow_cache((size_t)n + 1);

    /* Compute missing terms up to n. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];

        /* compute 6*a1 + 13*a2 + 2*a4 - 4*a3 safely in unsigned arithmetic */
        seq_t term = a1 * 6 + a2 * 13 + a4 * 2;
        term -= a3 * 4;
        cache[i] = term;
    }
    max_cached = n > max_cached ? n : max_cached;

    return cache[n];
}
