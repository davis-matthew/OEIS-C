/*
 * A093303  a(n) = a(n-1)*(2n-1) + 2n with a(0)=0.
 * https://oeis.org/A093303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)*(2*n-1) + 2*n, a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed values.  The cache is grown as
       needed; it is indexed exactly like the sequence (a(0) is cache[0]). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache == NULL) {          /* initialise with a(0) */
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)0;
    }

    if (n < cache_len)            /* already known */
        return cache[n];

    /* Grow the cache to hold indices up to n */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (tmp == NULL)              /* allocation failed – return 0 */
        return (seq_t)0;
    cache = tmp;

    /* Compute the new entries */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t mult = (seq_t)((seq_t)2 * (seq_t)i - (seq_t)1);
        seq_t add  = (seq_t)((seq_t)2 * (seq_t)i);
        cache[i] = prev * mult + add;
    }
    cache_len = n + 1;
    return cache[n];
}
