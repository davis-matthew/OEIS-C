/*
 * A186564  Number of (n+2) X 7 0..2 arrays with every 3 X 3 subblock
 *          commuting with each horizontal and vertical neighbor 3 X 3
 *          subblock.
 * https://oeis.org/A186564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            2*a(n-1)-a(n-2)+2*a(n-3)-4*a(n-4)+2*a(n-5)+2*a(n-6)-2*a(n-7) for
 *            n>13 initial values: a(1)=12122, a(2)=2617, a(3)=4807,
 *            a(4)=11557, a(5)=21007, a(6)=23389, a(7)=27391, a(8)=33445,
 *            a(9)=44263, a(10)=68269, a(11)=104335, a(12)=154693,
 *            a(13)=256359
 * Range:     u8 none, u16 n <= 9, u32 n <= 35, u64 n <= 87, u128 n <= 191,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = (seq_t *)0;
    static size_t cache_cap = 0;   /* capacity of cache array */
    static idx_t cache_max = 0;    /* largest n for which cache[n] is defined */
    static int base_init = 0;      /* flag indicating base values inserted */

    /* allocate initial cache if necessary (size 14, index 1..13 will be used) */
    if (!cache) {
        cache_cap = 14;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        for (size_t i = 0; i < cache_cap; ++i) {
            cache[i] = 0;
        }
    }

    /* set initial terms if not yet done */
    if (!base_init) {
        cache[1]  = 12122;
        cache[2]  = 2617;
        cache[3]  = 4807;
        cache[4]  = 11557;
        cache[5]  = 21007;
        cache[6]  = 23389;
        cache[7]  = 27391;
        cache[8]  = 33445;
        cache[9]  = 44263;
        cache[10] = 68269;
        cache[11] = 104335;
        cache[12] = 154693;
        cache[13] = 256359;
        cache_max = 13;
        base_init = 1;
    }

    /* if requested value already computed, return it */
    if (n <= cache_max) {
        return cache[n];
    }

    /* compute further values up to n */
    while (cache_max < n) {
        /* grow cache if necessary */
        if ((size_t)(cache_max + 1) >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        }
        idx_t i = cache_max + 1;

        /* recurrence applies for i > 13; for i <=13 values already set */
        seq_t val = 0;
        if (i > 13) {
            val = cache[i - 1] * 2;
            val -= cache[i - 2];
            val += cache[i - 3] * 2;
            val -= cache[i - 4] * 4;
            val += cache[i - 5] * 2;
            val += cache[i - 6] * 2;
            val -= cache[i - 7] * 2;
        }
        cache[i] = val;
        cache_max = i;
    }

    return cache[n];
}
