/*
 * A133331  a(2n+1) = 3a(2n)-3a(2n-1)+2a(2n-2), a(2n+2) = 3a(2n+1)-3a(2n),
 *          a(0) = 0, a(1) = 1, a(2) = 3.
 * https://oeis.org/A133331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 27, i32 n <= 27, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=3; 
   a(2n+1)=3a(2n)-3a(2n-1)+2a(2n-2);
   a(2n+2)=3a(2n+1)-3a(2n) */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t curmax = 2;   /* highest index that has been computed */
    static idx_t cache_size = 0; /* allocated size of cache */

    /* allocate initial cache and set base values */
    if (cache == NULL) {
        cache_size = 3; /* indices 0,1,2 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();        /* unrecoverable in this context */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
    }

    /* expand cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* compute values up to n if necessary */
    if (n > curmax) {
        for (idx_t i = curmax + 1; i <= n; ++i) {
            if (i % 2 == 0) { /* even: i = 2n+2 */
                cache[i] = (seq_t)3 * cache[i-1] - (seq_t)3 * cache[i-2];
            } else {           /* odd: i = 2n+1 */
                cache[i] = (seq_t)3 * cache[i-1] - (seq_t)3 * cache[i-2]
                           + (seq_t)2 * cache[i-3];
            }
        }
        curmax = n;
    }

    return cache[n];
}
