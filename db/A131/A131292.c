/*
 * A131292  a(2n+1)=3a(2n)-3a(2n-1)+2a(2n-2), a(2n+2)=3a(2n+1)-3a(2n),
 *          a(0)=a(1)=a(2)=1.
 * https://oeis.org/A131292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 16, i16 n <= 38, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(2n+1) = 3a(2n) - 3a(2n-1) + 2a(2n-2),
   a(2n+2) = 3a(2n+1) - 3a(2n),
   a(0) = a(1) = a(2) = 1. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_computed = 0;  /* largest index that has been filled */

    /* Make sure we have room for index n and for indices 0..2. */
    size_t need = (size_t)(n + 1);
    if (need < 3) need = 3;
    if (cache == NULL) {
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        cache_size = need;
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        max_computed = 2;
    } else if (cache_size < need) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        cache = tmp;
        cache_size = need;
    }

    /* Return if already computed. */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Compute missing values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i & 1U) {                     /* odd index: a(2n+1) */
            cache[i] = 3 * cache[i - 1]
                     - 3 * cache[i - 2]
                     + 2 * cache[i - 3];
        } else {                           /* even index: a(2n+2) */
            cache[i] = 3 * cache[i - 1]
                     - 3 * cache[i - 2];
        }
    }
    max_computed = n;
    return cache[n];
}
