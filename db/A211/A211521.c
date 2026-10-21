/*
 * A211521  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w + 2x = 4y.
 * https://oeis.org/A211521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-4) - a(n-5) - a(n-6) +
 *            a(n-7), * with a(0)=0, a(1)=0, a(2)=1, a(3)=2, a(4)=4, a(5)=5,
 *            a(6)=9
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for previously computed values.  The array is grown on demand,
 * and the cached values are kept between calls. */
static seq_t *cache = NULL;
static idx_t  cache_n = 0;   /* highest index that has been computed in cache */
static size_t cache_cap = 0; /* number of slots actually allocated */

/* Return the number of ordered triples (w,x,y) with 1≤w,x,y≤n and w+2x=4y. */
seq_t a(idx_t n)
{
    /* Base values for n = 0..6 */
    static const seq_t base[7] = {0, 0, 1, 2, 4, 5, 9};

    /* If n is small, we can answer directly without using the cache. */
    if (n <= 6)
        return base[n];

    /* Ensure the cache has enough capacity to hold a[0..n]. */
    if (cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* Allocation failed – abort. */
            /* In a real library we could handle this differently. */
            exit(1);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n.  We fill from the largest
       index already computed (cache_n) + 1 up to n inclusive. */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        if (i <= 6) {             /* safety path – not needed after n>6 */
            cache[i] = base[i];
        } else {
            seq_t term
                = cache[i-1] + cache[i-2]
                  - cache[i-3] + cache[i-4]
                  - cache[i-5] - cache[i-6]
                  + cache[i-7];
            cache[i] = term;
        }
    }

    /* Update the cache's highest computed index. */
    if (cache_n < n)
        cache_n = n;

    return cache[n];
}
