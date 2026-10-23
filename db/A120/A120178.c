/*
 * A120178  a(n)=ceiling( sum_{i=1..n-1} a(i)/6), a(1)=1.
 * https://oeis.org/A120178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n)=ceil( sum_{i=1}^{n-1} a(i)/6 )
 * Range:     u8 n <= 28, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined for n==0; return 0 for safety. */
    if (n == 0)
        return 0;

    /* Static memoisation state. */
    static seq_t *cache  = NULL;
    static idx_t cache_size = 0;   /* number of valid terms stored */
    static idx_t cache_cap   = 0;   /* allocated capacity */
    static seq_t cum_sum = 0;       /* cumulative sum of stored terms */

    /* Initialise cache with a(1) = 1. */
    if (!cache) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache)
            return 0;
        cache[0] = 1;
        cum_sum = 1;
        cache_size = 1;
    }

    /* Grow the array if needed. */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp)
            return 0;   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any terms that have not yet been computed. */
    for (idx_t i = cache_size; i < n; ++i) {
        seq_t val = cum_sum / 6;
        if (cum_sum % 6 != 0)
            ++val;
        cache[i] = val;
        cum_sum += val;
    }
    cache_size = n;

    return cache[n - 1];
}
