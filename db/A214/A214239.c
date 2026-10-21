/*
 * A214239  Number of 0..7 colorings of a 1 X (n+1) array circular in the n+1
 *          direction with new values 0..7 introduced in row major order.
 * https://oeis.org/A214239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = recurrence a(1)=1, a(2)=1, a(3)=4, a(4)=11, a(5)=41,
 *            a(6)=162, a(7)=715,
 *            a(n)=21*a(n-1)-168*a(n-2)+630*a(n-3)-1029*a(n-4)+189*a(n-5)+1198*a(n-6)-840*a(n-7)
 *            for n>7
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 27, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;
    static const seq_t seeds[8] = {0, 1, 1, 4, 11, 41, 162, 715};

    if (n < 1)
        return 0;

    /* Expand cache if needed */
    if (n > cache_len) {
        size_t required = (size_t)(n + 1);
        if (required > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < required)
                new_cap = required;
            seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!newp)               /* allocation failed – return the last known value */
                return cache_len ? cache[cache_len] : 0;
            cache = newp;
            cache_cap = new_cap;
        }

        /* Compute missing values */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 7) {
                cache[i] = seeds[i];
            } else {
                cache[i] = (seq_t)21 * cache[i - 1]
                         - (seq_t)168 * cache[i - 2]
                         + (seq_t)630 * cache[i - 3]
                         - (seq_t)1029 * cache[i - 4]
                         + (seq_t)189 * cache[i - 5]
                         + (seq_t)1198 * cache[i - 6]
                         - (seq_t)840 * cache[i - 7];
            }
        }
        cache_len = n;
    }
    return cache[n];
}
