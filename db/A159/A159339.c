/*
 * A159339  Transform of A056594 by the T_{1,0} transformation (see link).
 * https://oeis.org/A159339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 4*a(n-3) - 2*a(n-4) + a(n-5)
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=11, a(4)=27
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache with the first five terms */
    if (cache_len == 0) {
        cache_len = 5;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) /* allocation failure – return 0 as fallback */
            return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)27;
    }

    /* extend cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) /* allocation failure – return last known value */
            return cache[cache_len - 1];
        cache = tmp;
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t t = (seq_t)3 * cache[i-1]
                    - (seq_t)3 * cache[i-2]
                    + (seq_t)4 * cache[i-3]
                    - (seq_t)2 * cache[i-4]
                    + cache[i-5];
            cache[i] = t;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
