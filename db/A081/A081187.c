/*
 * A081187  5th binomial transform of (1,0,1,0,1,...), A059841.
 * https://oeis.org/A081187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 24*a(n-2), a(0)=1, a(1)=5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t capacity = 0;      /* number of slots allocated           */
    static idx_t length   = 0;      /* number of terms initialized         */

    /* initialise cache with the first two terms */
    if (length == 0) {
        capacity = 2;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 5;
        length = 2;
    }

    /* enough terms already computed? */
    if (n < length)
        return cache[n];

    /* grow array if necessary */
    if (n >= capacity) {
        size_t newcap = capacity;
        while (n >= newcap)
            newcap *= 2;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        capacity = newcap;
    }

    /* compute missing terms */
    for (idx_t i = length; i <= n; ++i) {
        cache[i] = 10 * cache[i-1] - 24 * cache[i-2];
    }
    length = n + 1;
    return cache[n];
}
