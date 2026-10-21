/*
 * A205610  Number of 3 X (n+1) 0..3 arrays with every 2 X 2 subblock having
 *          the same number of equal edges as its horizontal neighbors and a
 *          different number from its vertical neighbors, and new values 0..3
 *          introduced in row major order.
 * https://oeis.org/A205610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*a(n-1) +31*a(n-2) -1380*a(n-3) +24*a(n-4)
 *            +37716*a(n-5) + 28069*a(n-6) -327857*a(n-7) -359875*a(n-8)
 *            +1111408*a(n-9) + 1460528*a(n-10) -1281648*a(n-11)
 *            -2003792*a(n-12) +11264*a(n-13) + 365568*a(n-14) for n ≥ 15 a(1)
 *            = 110, a(2) = 1030, a(3) = 9688, a(4) = 95564, a(5) = 946066,
 *            a(6) = 9...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values: cache[i] = a(i+1) for i >= 0 */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity    */
    static size_t cache_cnt = 0;   /* number of values cached */

    if (n == 0) return 0; /* undefined, but safe guard */

    /* Ensure allocation large enough */
    size_t needed = (size_t)n;
    if (needed < 14u) needed = 14u;   /* always store first 14 terms */
    if (cache_cap < needed)
    {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                             needed * sizeof(seq_t));
        cache = new_cache;
        cache_cap = needed;
    }

    /* Initialize first 14 terms if not already done */
    if (cache_cnt < 14u)
    {
        static const seq_t init[14] = {
            110ULL, 1030ULL, 9688ULL, 95564ULL, 946066ULL,
            9502090ULL, 96151010ULL, 982430426ULL, 10112195016ULL,
            104859909328ULL, 1094042468862ULL, 11478802373122ULL,
            121006212428726ULL, 1280887123146782ULL
        };
        size_t i;
        for (i = cache_cnt; i < 14u && i < needed; ++i)
            cache[i] = init[i];
        if (i > cache_cnt)
            cache_cnt = i;
    }

    /* Recurrence coefficients */
    static const int  coeff_sign[14] = {
        1,  1, -1,  1,  1,  1, -1, -1,  1,  1, -1, -1,  1,  1
    };
    static const seq_t coeff_abs[14] = {
        17ULL, 31ULL, 1380ULL, 24ULL, 37716ULL, 28069ULL,
        327857ULL, 359875ULL, 1111408ULL, 1460528ULL,
        1281648ULL, 2003792ULL, 11264ULL, 365568ULL
    };

    /* Compute terms up to n (index n-1) */
    if (cache_cnt < (size_t)n) {
        size_t i;
        for (i = cache_cnt; i < (size_t)n; ++i) {
            seq_t pos_sum = 0;
            seq_t neg_sum = 0;
            /* use a(i+1) = a( i ) + ...  */
            int t;
            for (t = 0; t < 14; ++t) {
                seq_t prod = coeff_abs[t] * cache[i - (t + 1)];
                if (coeff_sign[t] > 0)
                    pos_sum += prod;
                else
                    neg_sum += prod;
            }
            cache[i] = pos_sum - neg_sum; /* pos_sum >= neg_sum */
        }
        cache_cnt = (size_t)n;
    }

    return cache[n - 1];
}
