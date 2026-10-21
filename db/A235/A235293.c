/*
 * A235293  Number of (n+1) X (3+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 17*a(n-2) – 57*a(n-3) – 86*a(n-4) + 372*a(n-5)
 *            + 76*a(n-6) – 972*a(n-7) + 360*a(n-8) + 864*a(n-9) – 576*a(n-10)
 *            Initial terms: a(1)=272, a(2)=476, a(3)=832, a(4)=1736,
 *            a(5)=3536, a(6)=8336, a(7)=18832, a(8)=48128, a(9)=115952,
 *            a(10)=312416
 * Range:     u8 none, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values: cache[i] holds a(i+1) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (n == 0) return 0;              /* offset starts at 1 – behaviour for n<1 irrelevant */

    if (cache == NULL) {
        cache = (seq_t*)malloc(10 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 272ULL;
        cache[1] = 476ULL;
        cache[2] = 832ULL;
        cache[3] = 1736ULL;
        cache[4] = 3536ULL;
        cache[5] = 8336ULL;
        cache[6] = 18832ULL;
        cache[7] = 48128ULL;
        cache[8] = 115952ULL;
        cache[9] = 312416ULL;
        cache_size = 10;
    }

    if ((size_t)n > cache_size) {
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (size_t i = cache_size; i < new_size; ++i) {
            seq_t pos_sum = 0;
            seq_t neg_sum = 0;
            /* j = 1..10 corresponds to a(n-1) .. a(n-10) */
            seq_t t1 = cache[i-1];
            seq_t t2 = cache[i-2];
            seq_t t3 = cache[i-3];
            seq_t t4 = cache[i-4];
            seq_t t5 = cache[i-5];
            seq_t t6 = cache[i-6];
            seq_t t7 = cache[i-7];
            seq_t t8 = cache[i-8];
            seq_t t9 = cache[i-9];
            seq_t t10 = cache[i-10];

            pos_sum += t1 * (seq_t)3;
            pos_sum += t2 * (seq_t)17;
            neg_sum += t3 * (seq_t)57;
            neg_sum += t4 * (seq_t)86;
            pos_sum += t5 * (seq_t)372;
            pos_sum += t6 * (seq_t)76;
            neg_sum += t7 * (seq_t)972;
            pos_sum += t8 * (seq_t)360;
            pos_sum += t9 * (seq_t)864;
            neg_sum += t10 * (seq_t)576;

            cache[i] = pos_sum - neg_sum;  /* modulo arithmetic on `seq_t` */
        }
        cache_size = new_size;
    }

    return cache[n-1];
}
