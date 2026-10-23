/*
 * A035274  One fifth of deca-factorial numbers.
 * https://oeis.org/A035274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{j=1..n} (10*j-5) = ∏_{j=1..n} 5*(2*j-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Compute sequence A035274.  a(1)=1. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* computed terms */
    static idx_t cache_cap = 0;       /* current capacity of cache */
    static idx_t cache_len = 0;       /* number of terms stored */

    if (n == 0)            /* undefined for n<1; return 0 */
        return (seq_t)0;

    /* grow the cache if needed */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);            /* allocation failed */
        if (cache) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* initialise first term if cache is empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;          /* a(1) = 1 */
        cache_len = 1;
    }

    /* compute any missing terms up to n */
    if (n > cache_len) {
        for (idx_t j = cache_len + 1; j <= n; ++j) {
            seq_t factor = (seq_t)5 * ( (seq_t)(2 * j - 1) );
            cache[cache_len] = cache[cache_len - 1] * factor;
            ++cache_len;
        }
    }

    return cache[n - 1];
}
