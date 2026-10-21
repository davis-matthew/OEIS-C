/*
 * A205635  Number of (n+1)X2 0..3 arrays with every 2 X 2 subblock having the
 *          number of clockwise edge increases equal to the number of
 *          counterclockwise edge increases in its adjacent leftward and
 *          upward neighbors.
 * https://oeis.org/A205635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 64*a(n-2) - 46*a(n-3) + 680*a(n-4) -
 *            1040*a(n-5) - 73*a(n-6) + 912*a(n-7) - 228*a(n-8) - 176*a(n-9) +
 *            20*a(n-10) Initial values:
 *            a(1)=256,a(2)=2320,a(3)=21416,a(4)=198688,a(5)=1843744,
 *            a(6)=17111320,a(7)=158804736,a(8)=1473828208,
 *            a(9)=13678225256,a(10)=126944238720
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* a(1) at index 0 */
static size_t cache_len = 0;  /* number of terms currently stored */
static size_t cache_cap = 0;  /* allocated capacity of cache */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; a(0) undefined */

    size_t required = (size_t)n;   /* how many terms we need */

    /* Ensure enough capacity */
    if (required > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < required) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first ten terms only once */
    if (cache_len < 10) {
        static const seq_t init[10] = {
            (seq_t)256,          (seq_t)2320,
            (seq_t)21416,        (seq_t)198688,
            (seq_t)1843744,      (seq_t)17111320,
            (seq_t)158804736,    (seq_t)1473828208,
            (seq_t)13678225256,  (seq_t)126944238720
        };
        size_t init_len = (cache_cap < 10 ? cache_cap : 10);
        size_t i;
        for (i = cache_len; i < init_len; ++i) cache[i] = init[i];
        if (i > cache_len) cache_len = i;
    }

    /* If we already have the needed term, return it */
    if (n <= cache_len) return cache[n - 1];

    /* Compute missing terms using the recurrence */
    for (size_t i = cache_len; i < required; ++i) {
        seq_t plus  = (seq_t)16  * cache[i - 1]
                    + (seq_t)680 * cache[i - 4]
                    + (seq_t)912 * cache[i - 7]
                    + (seq_t)20  * cache[i - 10];
        seq_t minus = (seq_t)64  * cache[i - 2]
                    + (seq_t)46  * cache[i - 3]
                    + (seq_t)1040 * cache[i - 5]
                    + (seq_t)73  * cache[i - 6]
                    + (seq_t)228 * cache[i - 8]
                    + (seq_t)176 * cache[i - 9];
        cache[i] = plus - minus;
    }
    cache_len = required;

    return cache[n - 1];
}
