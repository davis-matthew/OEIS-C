/*
 * A014447  Odd Lucas numbers.
 * https://oeis.org/A014447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2)+a(n-4); a(0)=1, a(1)=3, a(2)=7, a(3)=11
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;   /* number of stored elements (max index + 1) */

    /* handle small n directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)11;

    /* if already computed, return */
    if (cache && n < cache_len)
        return cache[(size_t)n];

    /* ensure capacity */
    size_t required = (size_t)n + 1;
    if (!cache || required > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap << 1 : 8;
        if (new_cap < required) new_cap = required;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing values */
    for (size_t i = (size_t)cache_len; i <= (size_t)n; ++i) {
        if (i < 4) {
            /* initial terms */
            switch ((idx_t)i) {
                case 0: cache[i] = (seq_t)1; break;
                case 1: cache[i] = (seq_t)3; break;
                case 2: cache[i] = (seq_t)7; break;
                case 3: cache[i] = (seq_t)11; break;
            }
        } else {
            /* recurrence: a(i) = 4*a(i-2)+a(i-4) */
            cache[i] = (seq_t)4 * cache[i-2] + cache[i-4];
        }
    }
    cache_len = (idx_t)(n + 1);
    return cache[(size_t)n];
}
