/*
 * A045693  Number of binary words of length n (beginning 0) with
 *          autocorrelation function 2^(n-1)+3.
 * https://oeis.org/A045693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(2) = 0; a(3) = 1, a(4) = 0; For n ≥ 3: a(2n-1) =
 *            2*a(2n-2) + a(n) a(2n) = 2*a(2n-1) + a(n) - a(n+1)
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache   = NULL;
    static size_t  cache_size = 0;  /* largest index currently available */

    /* size of cache before we ensure it has room for n */
    size_t old_size = cache_size;

    /* grow or create cache as needed */
    if (cache == NULL) {
        /* initialise cache up to at least 3 so that the first three
           terms can be filled directly */
        cache_size = (n > 3) ? n : 3;
        cache = (seq_t *)calloc(cache_size + 1, sizeof(seq_t));
        old_size = 0;   /* nothing was initialised yet */
    } else if ((idx_t)n > (idx_t)cache_size) {
        cache = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        old_size = cache_size;
        cache_size = (size_t)n;
    } else {
        old_size = cache_size;  /* nothing to grow, keep old size */
    }

    /* compute any missing values from old_size+1 up to cache_size */
    for (idx_t i = old_size + 1; i <= cache_size; ++i) {
        if (i == 1 || i == 2) {
            cache[i] = 0;
        } else if (i == 3) {
            cache[i] = 1;
        } else if (i == 4) {
            cache[i] = 0;
        } else if (i & 1) {                /* odd i = 2m-1, m≥3 */
            idx_t m = (i + 1) / 2;
            cache[i] = (seq_t)2 * cache[i - 1] + cache[m];
        } else {                           /* even i = 2m, m≥3 */
            idx_t m = i / 2;
            cache[i] = (seq_t)2 * cache[i - 1] + cache[m] - cache[m + 1];
        }
    }

    return cache[n];
}
