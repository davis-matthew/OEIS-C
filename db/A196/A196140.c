/*
 * A196140  Number of nX2 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 3,0,4,1,2 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A196140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-3) - 2*a(n-4) - a(n-5) + a(n-6) Initial
 *            values: a(1)=3, a(2)=5, a(3)=8, a(4)=17, a(5)=34, a(6)=62
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 65, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated array size (>= n+1) */
    static idx_t  computed = 0;     /* highest index already computed */

    /* allocate or grow cache to hold indices up to n */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* ensure base values are stored once */
    if (computed < 6) {
        /* resize to hold indices 1..6 (need size 7) */
        if (cache_size < 7) {
            seq_t *tmp = (seq_t *)realloc(cache, 7 * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_size = 7;
        }
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)17;
        cache[5] = (seq_t)34;
        cache[6] = (seq_t)62;
        computed = 6;
    }

    /* compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i < 7) continue;          /* these indices are already initialized */
        seq_t val = cache[i-1] + cache[i-1];   /* 2*a(i-1)       */
        val += cache[i-3];                     /* + a(i-3)       */
        val -= cache[i-4];                     /* - a(i-4)       */
        val -= cache[i-4];                     /* - a(i-4)       */
        val -= cache[i-5];                     /* - a(i-5)       */
        val += cache[i-6];                     /* + a(i-6)       */
        cache[i] = val;
    }
    if (n > computed) computed = n;

    return cache[n];
}
