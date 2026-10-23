/*
 * A190032  Number of (n+2) X 10 binary arrays with every 3 X 3 subblock
 *          commuting with each horizontal and vertical neighbor 3 X 3
 *          subblock.
 * https://oeis.org/A190032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) + a(n-6) -
 *            a(n-7) for n > 16. Initial values: a(1)=1508, a(2)=423,
 *            a(3)=610, a(4)=966, a(5)=1418, a(6)=1834, a(7)=2362, a(8)=3018,
 *            a(9)=3834, a(10)=5122, a(11)=6886, a(12)=9242, a(13)=12530,
 *            a(14)=16874, a(15)=22626, a(16)=30959
 * Range:     u8 none, u16 n <= 18, u32 n <= 54, u64 n <= 126, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle n < 1 */
    if (n == 0)
        return (seq_t)0;

    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initial values for indices 1..16 */
    static const seq_t init_vals[17] = {
        0,          /* index 0 unused */
        1508, 423,  610,  966, 1418, 1834, 2362, 3018, 3834, 5122,
        6886, 9242, 12530,16874,22626,30959
    };

    /* if cache is not yet allocated, create it with size 16 */
    if (cache == NULL) {
        cache_size = 16;
        cache = (seq_t *)calloc(cache_size + 1, sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;  /* allocation failed */
        for (idx_t i = 1; i <= cache_size; ++i)
            cache[i] = init_vals[i];
    }

    /* extend cache if needed */
    if (n > cache_size) {
        /* determine new size (grow at least twice) */
        idx_t new_size = cache_size;
        while (new_size < n)
            new_size = new_size * 2 + 1;   /* +1 to avoid zero */

        seq_t *tmp = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;  /* allocation failed */
        cache = tmp;
        /* compute new entries from cache_size+1 up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* recurrence: a(i) = 2*a(i-1) - a(i-2) + a(i-3) - 2*a(i-4)
                           + a(i-5) + a(i-6) - a(i-7) */
            seq_t val = 0;
            val += (cache[i-1] << 1);      /* 2*a(i-1) */
            val -= cache[i-2];
            val += cache[i-3];
            val -= (cache[i-4] << 1);      /* 2*a(i-4) */
            val += cache[i-5];
            val += cache[i-6];
            val -= cache[i-7];
            cache[i] = val;
        }
        cache_size = n;
    }

    return cache[n];
}
