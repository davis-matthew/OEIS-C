/*
 * A207718  Number of 3 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) - a(n-3) - 4*a(n-4) + a(n-5), a(1)=6,
 *            a(2)=36, a(3)=114, a(4)=450, a(5)=1644
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 66,
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

    if (n == 0) return 0;             /* not used, safe guard */

    if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!tmp) { /* Allocation failed: abort or handle error appropriately */
            /* In the typical build environment this shouldn't happen */
            abort();
        }
        cache = tmp;
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1)          cache[0] = (seq_t)6;
            else if (i == 2)      cache[1] = (seq_t)36;
            else if (i == 3)      cache[2] = (seq_t)114;
            else if (i == 4)      cache[3] = (seq_t)450;
            else if (i == 5)      cache[4] = (seq_t)1644;
            else {
                /* indices: a(i-1)=cache[i-2], a(i-2)=cache[i-3],
                           a(i-3)=cache[i-4], a(i-4)=cache[i-5],
                           a(i-5)=cache[i-6] */
                seq_t sum = (seq_t)2 * cache[i-2]            /* 2*a(i-1) */
                          + (seq_t)7 * cache[i-3]            /* 7*a(i-2) */
                          + cache[i-6];                    /* +a(i-5) */
                seq_t sub = cache[i-4] + (seq_t)4 * cache[i-5]; /* a(i-3)+4*a(i-4) */
                cache[i-1] = sum - sub;
            }
        }
        cache_size = n;
    }
    return cache[n-1];
}
