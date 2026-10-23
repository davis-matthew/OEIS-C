/*
 * A028196  Expansion of 1/((1-5x)(1-9x)(1-10x)(1-11x)).
 * https://oeis.org/A028196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=35, a(n)=21*a(n-1)-110*a(n-2)+(9^(n+1)-5^(n+1))/4
 *            for n>=2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for values and powers of 5 and 9 */
    static seq_t *cache = NULL;
    static seq_t *p5    = NULL;
    static seq_t *p9    = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* grow cache if needed */
    if ((size_t)n >= cache_len) {
        size_t target = (size_t)n + 1;
        if (target > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < target) new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            p5    = (seq_t *)realloc(p5,    new_cap * sizeof(seq_t));
            p9    = (seq_t *)realloc(p9,    new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        for (size_t i = cache_len; i < target; ++i) {
            if (i == 0) {
                cache[0] = 1;
                p5[0] = 1;
                p9[0] = 1;
            } else if (i == 1) {
                cache[1] = 35;
                p5[1] = p5[0] * 5;
                p9[1] = p9[0] * 9;
            } else {
                seq_t p5i = p5[i - 1] * 5;
                p5[i] = p5i;
                seq_t p9i = p9[i - 1] * 9;
                p9[i] = p9i;
                /* powers needed in the recurrence */
                seq_t p5next = p5i * 5;          /* 5^(i+1) */
                seq_t p9next = p9i * 9;          /* 9^(i+1) */
                seq_t part = (p9next - p5next) / 4;
                cache[i] = 21 * cache[i - 1] - 110 * cache[i - 2] + part;
            }
        }
        cache_len = target;
    }
    return cache[(size_t)n];
}
