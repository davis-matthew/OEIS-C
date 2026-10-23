/*
 * A188501  Number of n X 2 binary arrays without the pattern 0 1 0
 *          diagonally, vertically or horizontally.
 * https://oeis.org/A188501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + 3*a(n-3) + 3*a(n-4) - a(n-5) - a(n-6)
 *            Initial values: a(1) = 4 a(2) = 16 a(3) = 49 a(4) = 144 a(5) =
 *            441 a(6) = 1369
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;     /* array indexed from 0 to max_n inclusive */
    static idx_t cache_size = 0;    /* current allocated size of cache */
    static idx_t max_n = 0;         /* highest n that has been computed */

    if (n == 0) return 0;           /* undefined for n < 1, return 0 */

    /* allocate or extend cache if necessary */
    if (n > max_n) {
        idx_t new_size = n + 1;     /* we need indices 0..n */
        if (new_size > cache_size) {
            seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) abort();     /* allocation failed */
            cache = tmp;
            cache_size = new_size;
        }

        /* compute missing values from max_n+1 .. n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i <= 6) {                       /* initial terms */
                switch (i) {
                    case 1: cache[1] = (seq_t)4;   break;
                    case 2: cache[2] = (seq_t)16;  break;
                    case 3: cache[3] = (seq_t)49;  break;
                    case 4: cache[4] = (seq_t)144; break;
                    case 5: cache[5] = (seq_t)441; break;
                    case 6: cache[6] = (seq_t)1369;break;
                }
            } else {
                /* use the recurrence */
                seq_t s = cache[i-1];    /* 1*a(i-1) */
                s += cache[i-1];         /* 2*a(i-1) */
                s += cache[i-1];         /* 3*a(i-1) */
                s -= cache[i-2];         /* -a(i-2) */
                s += cache[i-3];         /* +a(i-3) */
                s += cache[i-3];         /* +2*a(i-3) */
                s += cache[i-3];         /* +3*a(i-3) */
                s += cache[i-4];         /* +a(i-4) */
                s += cache[i-4];         /* +2*a(i-4) */
                s += cache[i-4];         /* +3*a(i-4) */
                s -= cache[i-5];         /* -a(i-5) */
                s -= cache[i-6];         /* -a(i-6) */
                cache[i] = s;
            }
        }
        max_n = n;
    }

    return cache[n];
}
