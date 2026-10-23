/*
 * A165746  a(n) = 3 - 2*3^n.
 * https://oeis.org/A165746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3 - 2*3^n Recurrence used: a(0) = 1, a(n) = 3*a(n-1) - 6
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 18, i64 n <= 39, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Static cache for computed values  */
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;   /* number of elements stored   */
    static idx_t cache_cap = 0;   /* allocated capacity          */

    /*  Initialise cache with a(0)  */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();          /* cannot happen in tests    */
        cache[0] = (seq_t)1;                 /* a(0) = 1                  */
        cache_len = 1;
    }

    /*  Grow cache if needed  */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_ptr == NULL) abort();
        cache = new_ptr;
        cache_cap = new_cap;
    }

    /*  Compute missing terms  */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = (seq_t)3 * prev - (seq_t)6;   /* recurrence: 3*a(i-1)-6 */
    }
    cache_len = n + 1;

    return cache[n];
}
