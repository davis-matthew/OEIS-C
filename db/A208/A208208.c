/*
 * A208208  a(n)=(a(n-1)^4*a(n-2)+1)/a(n-3) with a(0)=a(1)=a(2)=1.
 * https://oeis.org/A208208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)^4 * a(n-2) + 1) / a(n-3), a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements currently cached */

    /* Ensure at least three entries in the cache with initial values 1 */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        cache_len = 3;
    }

    /* If the requested index is already cached, return it immediately */
    if (n < cache_len) {
        return cache[n];
    }

    /* Expand the cache up to n (inclusive) */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – abort gracefully */
        abort();
    }
    cache = tmp;

    /* Compute new terms in order, storing them in the cache */
    for (size_t i = cache_len; i < new_len; ++i) {
        /* Fetch the three previous values */
        seq_t a_nm3 = cache[i - 3];
        seq_t a_nm2 = cache[i - 2];
        seq_t a_nm1 = cache[i - 1];

        /* Compute a_nm1^4 * a_nm2 + 1, then divide by a_nm3 */
        seq_t temp = a_nm1 * a_nm1;   /* a_nm1^2 */
        temp = temp * a_nm1;          /* a_nm1^3 */
        temp = temp * a_nm1;          /* a_nm1^4 */
        seq_t numerator = temp * a_nm2 + (seq_t)1;
        cache[i] = numerator / a_nm3;
    }

    cache_len = new_len;
    return cache[n];
}
