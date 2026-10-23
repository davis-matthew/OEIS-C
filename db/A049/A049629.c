/*
 * A049629  a(n) = (F(6*n+5) - F(6*n+1))/4 = (F(6*n+4) + F(6*n+2))/4, where F
 *          = A000045.
 * https://oeis.org/A049629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - a(n-2), with a(0)=1, a(1)=19
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n < cache_len)
        return cache[n];

    idx_t old_len = cache_len;
    idx_t new_len = n + 1;

    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t) * new_len);
    } else {
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_len);
    }
    cache_len = new_len;

    /* Ensure the base values are stored */
    if (cache_len >= 1)
        cache[0] = (seq_t)1;
    if (cache_len >= 2)
        cache[1] = (seq_t)19;

    /* Compute the values that are missing */
    idx_t start = (old_len > 2) ? old_len : 2;
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = (seq_t)18 * cache[i-1] - cache[i-2];
    }

    return cache[n];
}
