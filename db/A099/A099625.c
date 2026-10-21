/*
 * A099625  a(n) = Sum_{k=0..floor(n/2)} C(n-k,k+2)*2^(n-k-2)*(1/2)^k.
 * https://oeis.org/A099625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 4*a(n-3) + 4*a(n-4)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=6 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cacheSize = 0;          /* number of stored elements */

    /* Ensure we have at least four entries initialized */
    if (cacheSize == 0) {
        cacheSize = 4;
        cache = (seq_t *)malloc(cacheSize * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;          /* out of memory – fail safely */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)6;
    }

    /* Expand the cache if needed */
    if (n + (idx_t)1 > cacheSize) {
        idx_t newSize = n + (idx_t)1;
        seq_t *tmp = (seq_t *)realloc(cache, newSize * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;          /* out of memory – fail safely */
        cache = tmp;

        /* Compute missing values using the recurrence */
        for (idx_t i = cacheSize; i <= n; ++i) {
            if (i < 4) {            /* safety: already initialized */
                continue;
            }
            cache[i] = 6 * cache[i-1]
                     - 11 * cache[i-2]
                     + 4 * cache[i-3]
                     + 4 * cache[i-4];
        }
        cacheSize = newSize;
    }

    return cache[n];
}
