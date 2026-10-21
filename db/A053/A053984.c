/*
 * A053984  a(n) = (2*n-1)*a(n-1) - a(n-2), a(0) = 0, a(1) = 1.
 * https://oeis.org/A053984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = (2*n-1)*a(n-1) - a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t   cache_size = 0;     /* allocated entries */
    static size_t   cache_len  = 0;     /* number of terms actually stored (>=1) */

    /* Lazy initialization */
    if (cache == NULL) {
        cache_size = 2;                 /* start with a(0) and a(1) */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) {            /* abort on allocation failure */
            exit(1);
        }
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;                  /* indices 0..1 are ready */
    }

    /* Expand cache if n is beyond what we have computed */
    if ((idx_t)cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            /* Compute a(i) = (2*i-1)*a(i-1) - a(i-2) */
            seq_t term = ((seq_t)(2 * i - 1)) * cache[i - 1] - cache[i - 2];

            /* Grow storage if necessary */
            if ((size_t)i >= cache_size) {
                cache_size *= 2;
                seq_t *newptr = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
                if (newptr == NULL) {
                    free(cache);
                    exit(1);
                }
                cache = newptr;
            }
            cache[i] = term;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
