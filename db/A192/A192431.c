/*
 * A192431  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) - 2*a(n-3) - a(n-4) with initial
 *            values a(0)=0, a(1)=1, a(2)=4, a(3)=15
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;   /* number of entries allocated */
    static idx_t computed    = 0;   /* highest index computed so far */

    /* initialise cache on first call */
    if (cache_size == 0) {
        cache_size = 4;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure (unlikely) */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)15;
        computed = 3;
    }

    /* enlarge cache if needed to reach index n */
    if ((idx_t)n + 1 > cache_size) {
        size_t new_size = (size_t)((idx_t)n + 1);
        seq_t *tmp = (seq_t *)realloc(cache,
                                      new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache      = tmp;
        cache_size = (idx_t)new_size;
        /* no need to initialise new entries yet */
    }

    /* compute missing terms using the recurrence */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t t = (seq_t)2 * cache[i - 1]
                    + (seq_t)6 * cache[i - 2]
                    - (seq_t)2 * cache[i - 3]
                    - cache[i - 4];
            cache[i] = t;
        }
        computed = n;
    }

    return cache[n];
}
