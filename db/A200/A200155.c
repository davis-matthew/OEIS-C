/*
 * A200155  Number of 0..n arrays x(0..3) of 4 elements with zero 3rd
 *          differences.
 * https://oeis.org/A200155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 4*a(n-4) + 2*a(n-5) -
 *            a(n-6) + 2*a(n-7) - a(n-8). Initial values: a(1)=4, a(2)=9,
 *            a(3)=22, a(4)=41, a(5)=66, a(6)=107, a(7)=158, a(8)=219
 * Range:     u8 n <= 8, u16 n <= 59, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    static seq_t *cache = NULL;
    static idx_t cached_len = 0;

    /* Allocate and initialise base terms on first call */
    if (cache == NULL) {
        idx_t initlen = 8 > 1 ? 8 : 1;            /* at least 8 slots (index 1..8) */
        cache = (seq_t *)malloc((initlen + 1) * sizeof(seq_t));
        cache[0] = 0;                             /* unused */
        cache[1] = 4;
        cache[2] = 9;
        cache[3] = 22;
        cache[4] = 41;
        cache[5] = 66;
        cache[6] = 107;
        cache[7] = 158;
        cache[8] = 219;
        cached_len = 8;
    }

    /* Extend cache if necessary */
    if (n > cached_len) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));

        for (idx_t i = cached_len + 1; i <= n; ++i) {
            if (i <= 8) {
                /* base values already set */
                continue;
            }

            seq_t val = (seq_t)2 * cache[i-1]
                        - cache[i-2]
                        + (seq_t)2 * cache[i-3]
                        - (seq_t)4 * cache[i-4]
                        + (seq_t)2 * cache[i-5]
                        - cache[i-6]
                        + (seq_t)2 * cache[i-7]
                        - cache[i-8];
            cache[i] = val;
        }
        cached_len = n;
    }

    return cache[n];
}
