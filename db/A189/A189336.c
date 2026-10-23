/*
 * A189336  Number of nX2 binary arrays without the pattern 1 0 0 1
 *          diagonally, vertically, antidiagonally or horizontally.
 * https://oeis.org/A189336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) -6*a(n-3) +12*a(n-4) -3*a(n-5) *
 *            +3*a(n-6) -6*a(n-7) + a(n-8) - a(n-9) + a(n-10); * Initial
 *            values: * a(1)=4, a(2)=16, a(3)=64, a(4)=225, a(5)=784,
 *            a(6)=2704, * a(7)=9409, a(8)=32761, a(9)=114244, a(10)=398161
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n<1 */

    static seq_t *cache = NULL;
    static idx_t cache_upto = 0;   /* highest index computed in cache */
    static int initialized = 0;    /* once flag for base values */

    /* initialize cache and base terms on first call */
    if (!initialized) {
        /* allocate at least for indices 0..10 */
        cache = (seq_t*)calloc(11, sizeof(seq_t));
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)16;
        cache[3] = (seq_t)64;
        cache[4] = (seq_t)225;
        cache[5] = (seq_t)784;
        cache[6] = (seq_t)2704;
        cache[7] = (seq_t)9409;
        cache[8] = (seq_t)32761;
        cache[9] = (seq_t)114244;
        cache[10] = (seq_t)398161;
        cache_upto = 10;
        initialized = 1;
    }

    /* grow cache if needed */
    if (n > cache_upto) {
        size_t new_cap = (size_t)n + 1;          /* indices 0..n */
        if (cache == NULL) {
            cache = (seq_t*)calloc(new_cap, sizeof(seq_t));
        } else if (new_cap > 0) {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)
                abort();                         /* allocation failure */
            cache = tmp;
        }
        /* compute missing values */
        for (idx_t i = cache_upto + 1; i <= n; ++i) {
            seq_t val =
                (seq_t)4 * cache[i-1]
                - cache[i-2]
                - (seq_t)6 * cache[i-3]
                + (seq_t)12 * cache[i-4]
                - (seq_t)3 * cache[i-5]
                + (seq_t)3 * cache[i-6]
                - (seq_t)6 * cache[i-7]
                + cache[i-8]
                - cache[i-9]
                + cache[i-10];
            cache[i] = val;
        }
        cache_upto = n;
    }

    return cache[n];
}
