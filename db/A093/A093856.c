/*
 * A093856  a(0)=1; a(1)=2; a(n+1) = 2*n*a(n) - a(n-1) for n >= 1.
 * https://oeis.org/A093856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; a(1) = 2; a(n+1) = 2*n*a(n) - a(n-1) for n ≥ 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0; /* number of elements currently stored */

    /* Initialise cache with the first two terms */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
    }

    /* If requested term is already cached, return it */
    if (n < cache_len) return cache[n];

    /* Grow cache to hold terms up to n */
    if (cache_len < n + 1) {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                ((size_t)n + 1) * sizeof(seq_t));
        if (new_cache == NULL) {
            /* allocation failed – leave cache unchanged */
            return 0;
        }
        cache = new_cache;

        /* Compute any missing terms iteratively */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* a(i) = 2*(i-1)*a(i-1) - a(i-2)  for i >= 2 */
            seq_t prod = ((seq_t)(i - 1u)) * cache[i - 1];
            prod *= (seq_t)2;
            cache[i] = prod - cache[i - 2];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
