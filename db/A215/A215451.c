/*
 * A215451  a(0)=1, a(n) = (sum of previous terms) mod (a(n-1)+n).
 * https://oeis.org/A215451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 598, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n≥1,
   a(n) = (∑_{k=0}^{n-1} a(k)) mod (a(n-1)+n) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed values and running prefix sum */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;    /* number of elements stored */
    static seq_t sum = 0;           /* sum of all stored elements */

    /* If we have not computed up to n, extend cache */
    if (n >= cache_len) {
        size_t newlen = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, newlen * sizeof(seq_t));

        /* Compute missing terms */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i == 0) {
                /* Base case */
                cache[0] = 1;
                sum = 1;
            } else {
                seq_t denom = cache[i-1] + (seq_t)i;
                cache[i] = sum % denom;
                sum += cache[i];
            }
        }
        cache_len = newlen;
    }
    return cache[n];
}
