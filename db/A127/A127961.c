/*
 * A127961  a(n) = A007583(n) written in binary.
 * https://oeis.org/A127961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, less, base
 * Formula:   a(0)=1, a(1)=11; a(n)=101*a(n-1)-100*a(n-2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 14,
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
    idx_t i;

    /* Ensure the cache contains at least the first two values. */
    if (cache_size < 2) {
        idx_t new_size = (n > 1 ? n + 1 : 2);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;           /* Allocation failure – return 0 */
        cache = tmp;
        cache_size = new_size;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        for (i = 2; i < cache_size; ++i) {
            seq_t t1 = cache[i - 1] * (seq_t)101;
            seq_t t2 = cache[i - 2] * (seq_t)100;
            cache[i] = t1 - t2;
        }
    }

    if (n < cache_size)
        return cache[n];

    /* Grow the cache to accommodate n. */
    idx_t old_size = cache_size;
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;           /* Allocation failure – return 0 */
    cache = tmp;
    cache_size = new_size;

    /* Compute new terms from old_size up to n. */
    for (i = old_size; i <= n; ++i) {
        seq_t t1 = cache[i - 1] * (seq_t)101;
        seq_t t2 = cache[i - 2] * (seq_t)100;
        cache[i] = t1 - t2;
    }

    return cache[n];
}
