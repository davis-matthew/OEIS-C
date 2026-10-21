/*
 * A213655  Number of dominating subsets of the theta-graph TH(2,2,n) (n>=1).
 *          A tribonacci sequence with initial values 13, 23, and 41.
 * https://oeis.org/A213655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3); a(1)=13, a(2)=23, a(3)=41
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;           /* number of computed terms (>=0) */

    /* Ensure the base terms are available */
    if (cache_len == 0)
    {
        cache_len = 3;
        cache = (seq_t*)malloc((cache_len + 1) * sizeof(seq_t));
        if (cache == NULL) return 0;      /* fallback, should not happen */
        cache[1] = 13;
        cache[2] = 23;
        cache[3] = 41;
    }

    /* If the requested term is already cached, return it */
    if (n <= cache_len)
        return cache[n];

    /* Resize cache to hold up to n */
    cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (cache == NULL) return 0;          /* fallback, should not happen */

    /* Compute missing terms */
    for (idx_t i = cache_len + 1; i <= n; ++i)
        cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3];

    cache_len = n;
    return cache[n];
}
