/*
 * A210675  a(n)=a(n-1)+a(n-2)+n+4, a(0)=0, a(1)=1.
 * https://oeis.org/A210675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+n+4, a(0)=0, a(1)=1
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;

    if (n < 2)
        return n == 0 ? (seq_t)0 : (seq_t)1;

    if (!cache) {
        cached_size = 2;
        cache = (seq_t *)malloc(cached_size * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;                     /* allocation failure fallback */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    if (n >= cached_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return cache[n];                     /* keep old cache on realloc failure */
        cache = tmp;

        for (idx_t i = cached_size; i <= n; ++i)
            cache[i] = cache[i - 1] + cache[i - 2] + (seq_t)i + (seq_t)4;

        cached_size = new_size;
    }

    return cache[n];
}
