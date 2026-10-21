/*
 * A207929  Number of 3 X n 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-3) + a(n-4), a(1)=6, a(2)=36, a(3)=72,
 *            a(4)=144, a(5)=360
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* stored values: cache[0] = a(1), cache[1] = a(2), ... */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;

    if (n <= max_computed) {
        return cache[n - 1];
    }

    /* resize cache if necessary */
    if (n > cap) {
        size_t newcap = cap ? cap : 1;
        while (newcap < (size_t)n)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) /* allocation failed, return 0 (undefined for this sequence) */
            return (seq_t)0;
        cache = tmp;
        cap = newcap;
    }

    /* fill in the missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i - 1] = (seq_t)6;
        } else if (i == 2) {
            cache[i - 1] = (seq_t)36;
        } else if (i == 3) {
            cache[i - 1] = (seq_t)72;
        } else if (i == 4) {
            cache[i - 1] = (seq_t)144;
        } else if (i == 5) {
            cache[i - 1] = (seq_t)360;
        } else {
            /* recurrence: a(i) = 2*a(i-1) + 2*a(i-3) + a(i-4) */
            seq_t val = cache[i - 2] + cache[i - 2]      /* 2*a(i-1) */
                      + cache[i - 4] + cache[i - 4]      /* 2*a(i-3) */
                      + cache[i - 5];                   /* a(i-4)   */
            cache[i - 1] = val;
        }
    }
    max_computed = n;
    return cache[n - 1];
}
