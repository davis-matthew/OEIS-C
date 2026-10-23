/*
 * A196876  a(n) = a(n-no-1)+....+a(n-1)+(n-no-2) where no is the 'no+1'th
 *          order of the series and 'n' is the element number, here no=6.
 * https://oeis.org/A196876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=1 for n<=7; a(n) = (n-8)+sum_{i=1..7}a(n-i)
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 37, u64 n <= 69, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;     /* 1‑based index, cache[1] == a(1) */
    static idx_t cache_size = 0;    /* allocated size (n+1) */
    static idx_t cached_max = 0;    /* highest n that has been computed */

    /* constants for this sequence */
    const idx_t NO = 6;
    const idx_t K = NO + 1;        /* number of terms in the sum */
    const idx_t PREFIX = NO + 1;   /* first (no+1) terms are 1 */

    /* extend cache if required */
    if (n > cached_max) {
        /* allocate or grow the array to hold indices up to n */
        idx_t new_cap = n + 1;      /* we ignore index 0 */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!cache) exit(1);
            cache_size = new_cap;
        } else if (new_cap > cache_size) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cache_size = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            if (i <= PREFIX) {
                cache[i] = (seq_t)1;
            } else {
                seq_t sum = 0;
                for (idx_t j = 1; j <= K; ++j)
                    sum += cache[i - j];
                cache[i] = (seq_t)(i - NO - 2) + sum;
            }
        }
        cached_max = n;
    }

    return cache[n];
}
