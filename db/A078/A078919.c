/*
 * A078919  Partial products of A079069.
 * https://oeis.org/A078919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-2)+a(n-3))*a(n-1)/a(n-4), n>4 a(1)=a(2)=a(3)=a(4)=1
 *            1‑based indexing This memoised implementation returns a(idx_t)
 *            for any caller order and uses only the allowed operations
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 11, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for already computed terms (1‑based indexing). */
    static seq_t *cache     = NULL; /* array of a(1), a(2), ... */
    static size_t cache_size = 0;    /* number of terms stored */
    static size_t cache_cap  = 0;    /* allocated capacity */

    if (n == 0u)          /* undefined, but safe for caller code */
        return (seq_t)0;

    /* Ensure the cache is large enough for the required term. */
    size_t target = (size_t)n;

    /* Allocate initial storage if this is the first call. */
    if (cache == NULL) {
        cache_cap = 16;
        while (cache_cap < target)
            cache_cap <<= 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
    }
    /* Grow the storage if the requested index exceeds current capacity. */
    while (target > cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Compute any terms that have not yet been cached. */
    for (size_t k = cache_size + 1; k <= target; ++k) {
        /* The array index for a(k) is (k-1). */
        if (k <= 4u) {
            cache[k-1] = (seq_t)1;
        } else {
            /* a(k-2)  -> cache[k-3]
               a(k-3)  -> cache[k-4]
               a(k-1)  -> cache[k-2]
               a(k-4)  -> cache[k-5]   */
            seq_t sum  = cache[k-3] + cache[k-4];
            seq_t prod = sum * cache[k-2];
            seq_t res  = prod / cache[k-5];
            cache[k-1] = res;
        }
    }
    cache_size = target;   /* update cached size */

    return cache[target-1];
}
