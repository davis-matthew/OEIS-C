/*
 * A228577  The number of 1-length gaps in all possible covers of n-length
 *          line by 2-length segments.
 * https://oeis.org/A228577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2)+2*a(n-3)-a(n-4)-2*a(n-5)-a(n-6) for n>5 with
 *            initial values a(0)=0, a(1)=1, a(2)=0, a(3)=2, a(4)=2, a(5)=3
 * Range:     u8 n <= 16, u16 n <= 34, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    size_t needed = (size_t)(n + 1);

    if (cache_size < needed) {
        /* enlarge cache */
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (cache == NULL) {
            /* allocation failed – terminate */
            exit(1);
        }

        /* compute values from cache_size up to n */
        for (size_t i = cache_size; i < needed; ++i) {
            if (i == 0) {
                cache[0] = 0;
            } else if (i == 1) {
                cache[1] = 1;
            } else if (i == 2) {
                cache[2] = 0;
            } else if (i == 3) {
                cache[3] = 2;
            } else if (i == 4) {
                cache[4] = 2;
            } else if (i == 5) {
                cache[5] = 3;
            } else {
                /* recurrence for i >= 6 */
                seq_t val = cache[i - 2];
                val += cache[i - 2];   /* 2*a(i-2) */
                val += cache[i - 3];
                val += cache[i - 3];   /* 2*a(i-3) */
                val -= cache[i - 4];   /* -a(i-4) */
                val -= cache[i - 5];
                val -= cache[i - 5];   /* -2*a(i-5) */
                val -= cache[i - 6];   /* -a(i-6) */
                cache[i] = val;
            }
        }
        cache_size = needed;
    }

    return cache[n];
}
