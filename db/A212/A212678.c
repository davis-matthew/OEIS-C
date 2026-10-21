/*
 * A212678  Number of (w,x,y,z) with all terms in {1,...,n} and
 *          w+x+y=|x-y|+|y-z|.
 * https://oeis.org/A212678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A212678: Number of (w,x,y,z) with all terms in {1,…,n}
   and w+x+y = |x−y| + |y−z|
   Recurrence (valid for n ≥ 11):
       a(n) = a(n−1)+a(n−2)+a(n−3) − 2·a(n−4) − 2·a(n−5)
              + a(n−6)+a(n−7)+a(n−8) − a(n−9)
   Initial values (n = 0 … 10):
       0 , 0 , 0 , 0 , 5 , 14 , 28 , 53 , 87 , 130 , 191
   The function is memoised; each term is computed only once.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: a[0] .. a[cache_size-1] */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* The first 11 values of the sequence */
    static const seq_t init_vals[11] = {
        0, 0, 0, 0, 5, 14, 28, 53, 87, 130, 191
    };

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(11 * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure */
        for (idx_t i = 0; i < 11; ++i) cache[i] = init_vals[i];
        cache_size = 11;
    }

    /* Return cached value if already computed */
    if (n < cache_size) return cache[n];

    /* Remember old size before possibly reallocating */
    idx_t old_size = cache_size;

    /* Grow cache to include up to n if necessary */
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cache_size = n + 1;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = old_size; i <= n; ++i) {
        /* For i <= 10 values are already initialised */
        if (i <= 10) continue;

        seq_t pos = cache[i-1] + cache[i-2] + cache[i-3]
                  + cache[i-6] + cache[i-7] + cache[i-8];
        seq_t neg = cache[i-4] * 2 + cache[i-5] * 2 + cache[i-9];
        cache[i] = pos - neg;
    }

    return cache[n];
}
