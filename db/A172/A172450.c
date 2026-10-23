/*
 * A172450  The case S(2,0,-1) of the family of self-convolutive recurrences
 *          studied by Martin and Kearney.
 * https://oeis.org/A172450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(n) = 2 * n * a(n-1) - Σ_{k=1}^{n-1} a(k) * a(n-k) (n
 *            > 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for all computed indices
       (cache[0] is unused).  cached_n is the highest index
       for which the value is stored. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* current allocated length (>= cached_n+1) */
    static idx_t cached_n = 0;   /* highest n that has been computed */

    if (n < 1)
        return 0;               /* defined only for n >= 1 */

    /* Grow the cache if needed */
    size_t needed = (size_t)(n + 1);          /* we need indices 0..n */
    if (needed > cache_len) {
        size_t new_len = needed * 2;          /* overshoot for future calls */
        seq_t *tmp;
        if (cache == NULL)
            tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
        else
            tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute terms that have not yet been calculated */
    idx_t i;
    for (i = cached_n + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 1;
            continue;
        }
        seq_t sum = 0;
        idx_t k;
        for (k = 1; k < i; ++k)
            sum += cache[k] * cache[i - k];
        seq_t term = (seq_t)2 * (seq_t)i * cache[i - 1];
        cache[i] = term - sum;
    }
    cached_n = n;

    return cache[n];
}
