/*
 * A014730  Squares of odd Lucas numbers.
 * https://oeis.org/A014730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-2)+17*a(n-4)-a(n-6) initial values: a(0)=1,
 *            a(1)=9, a(2)=49, a(3)=121, a(4)=841, a(5)=2209
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialize with first six terms once */
    if (cache_len == 0) {
        cache_len = 6;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)49;
        cache[3] = (seq_t)121;
        cache[4] = (seq_t)841;
        cache[5] = (seq_t)2209;
    }

    /* if requested n already cached, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* determine number of terms required */
    size_t needed = (size_t)n + 1;

    /* grow cache if necessary */
    if (needed > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – return last known value */
            return cache[cache_len - 1];
        }
        cache = tmp;

        /* compute missing terms using the recurrence */
        for (size_t i = cache_len; i < needed; ++i) {
            cache[i] = (seq_t)17 * cache[i - 2] +
                       (seq_t)17 * cache[i - 4] -
                       cache[i - 6];
        }
        cache_len = needed;
    }

    return cache[n];
}
