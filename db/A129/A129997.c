/*
 * A129997  a(n) = 4*a(n-1) + (n-6)*a(n-2).
 * https://oeis.org/A129997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + (n-6)*a(n-2) Offset: 1 (a(1)=0, a(2)=1)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: indices 1..cache_computed are filled.
       Index 0 is unused. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated size (bytes of seq_t) */
    static idx_t cache_computed = 2;/* highest n for which cache[n] is valid */

    /* --- first‑time allocation --------------------------------------- */
    if (cache == NULL) {
        /* allocate space for indices 0..3 (size 4) */
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(1);             /* allocation failure */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        /* cache[0] is unused */
    }

    /* --- grow the cache if necessary --------------------------------- */
    if (n > cache_computed) {
        /* ensure allocated space covers index n */
        if (cache_size <= (size_t)n) {
            size_t new_size = cache_size;
            while (new_size <= (size_t)n) new_size <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) exit(1);          /* allocation failure */
            cache = tmp;
            cache_size = new_size;
        }

        /* compute missing terms */
        for (idx_t i = cache_computed + 1; i <= n; ++i) {
            seq_t prev1 = cache[i-1];
            seq_t prev2 = cache[i-2];
            seq_t val = 4 * prev1;                /* 4*a(i-1) */

            if (i >= 6) {                         /* add (i-6)*a(i-2) */
                val += (seq_t)(i - 6) * prev2;
            } else {                              /* subtract (6-i)*a(i-2) */
                val -= (seq_t)(6 - i) * prev2;
            }
            cache[i] = val;
        }
        cache_computed = n;
    }

    return cache[n];
}
