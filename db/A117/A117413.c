/*
 * A117413  Expansion of (1-x^2)/(1-2*x^2+4*x^3+x^4).
 * https://oeis.org/A117413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 34, i64 n <= 67, i128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117413
   Formula: a(0)=1, a(1)=0, a(2)=1, a(3)=-4
            a(n)=2*a(n-2)-4*a(n-3)-a(n-4)   (n≥4) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache      = NULL; /* dynamic array of terms   */
    static idx_t cache_size  = 0;     /* allocated array size     */
    static idx_t max_computed = 0;    /* highest index computed   */

    /* initialise cache with the first four terms on the first call */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)-4;
        cache_size   = 4;
        max_computed = 3;
    }

    /* grow cache if needed */
    if (cache_size <= n) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_size = n + 1;
    }

    /* compute any missing terms */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* recurrence: a(i) = 2*a(i-2) - 4*a(i-3) - a(i-4) */
        cache[i] = (seq_t)2 * cache[i - 2]
                   - (seq_t)4 * cache[i - 3]
                   - cache[i - 4];
    }

    /* update state */
    if (max_computed < n) max_computed = n;

    return cache[n];
}
