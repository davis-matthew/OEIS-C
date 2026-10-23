/*
 * A192385  a(n) = A192384(n)/2.
 * https://oeis.org/A192385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 8*a(n-2) - 4*a(n-3) - 4*a(n-4), with a(1) = 0,
 *            a(2) = 1, a(3) = 2, a(4) = 12. Offset: 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;  /* memoisation table (index starts at 1) */
static idx_t max_cached = 0; /* largest index already computed */

/* Compute A192385(a(n)), with external linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n=0, return 0 as guard */
        return (seq_t)0;

    /* Initialise cache at first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));  /* indices 0..4 */
        if (!cache) return (seq_t)0;                 /* allocation failure */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)12;
        max_cached = 4;
    }

    /* Return previously computed value if available */
    if (n <= max_cached)
        return cache[n];

    /* Grow cache to cover index n */
    size_t new_size = (size_t)(n + 1);  /* +1 to make index n accessible */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)                       /* allocation failure */
        return (seq_t)0;
    cache = new_cache;

    /* Compute missing terms using the recurrence */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t val = 2 * cache[i - 1]
                  + 8 * cache[i - 2]
                  - 4 * cache[i - 3]
                  - 4 * cache[i - 4];
        cache[i] = val;
    }

    max_cached = n;
    return cache[n];
}
