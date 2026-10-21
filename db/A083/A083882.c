/*
 * A083882  a(0)=1, a(1)=4, a(n)=8a(n-1)-13a(n-2), n>=2.
 * https://oeis.org/A083882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 13*a(n-2), a(0)=1, a(1)=4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < 2)
        return (seq_t)(n == 0 ? 1 : 4);

    /* initialise cache with the first two terms */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            return 0;                 /* allocation failure (unlikely) */
        cache[0] = 1;
        cache[1] = 4;
    }

    /* extend cache up to n if necessary */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return 0;                 /* allocation failure (unlikely) */
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = 8 * cache[i-1] - 13 * cache[i-2];
        }
        cache_size = new_size;
    }

    return cache[n];
}
