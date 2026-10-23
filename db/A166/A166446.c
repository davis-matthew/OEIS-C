/*
 * A166446  Period 12: repeat [1,1,1,1,0,0,-1,-1,-1,-1,0,0].
 * https://oeis.org/A166446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-2) – a(n-4) Initial terms: a(0)=1, a(1)=1, a(2)=1,
 *            a(3)=1, a(4)=0, a(5)=0
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for computed values. */
    static seq_t *cache      = NULL;   /* dynamically grown array       */
    static idx_t  cached_upto = 0;    /* largest index that has been computed */
    static size_t cache_size   = 0;    /* allocated length of the array      */

    /* If the cache is empty, allocate space for at least (n+1) entries.   */
    if (cache == NULL) {
        cache = (seq_t *)realloc(NULL, ((size_t)n + 1) * sizeof(seq_t));
        if (!cache) abort();                 /* allocation failure */
        cache_size = (size_t)n + 1;
        cache[0] = 1;                        /* a(0) = 1                   */
        cached_upto = 0;
    } else if ((size_t)n + 1 > cache_size) {
        cache = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (!cache) abort();
        cache_size = (size_t)n + 1;
    }

    /* Compute any values that have not yet been computed. */
    if (n > cached_upto) {
        for (idx_t i = cached_upto + 1; i <= n; ++i) {
            if (i < 6) {
                /* First six terms from the definition   */
                cache[i] = (i <= 3) ? 1 : 0;
            } else {
                cache[i] = cache[i - 2] - cache[i - 4];
            }
        }
        cached_upto = n;
    }

    return cache[n];
}
