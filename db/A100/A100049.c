/*
 * A100049  A Chebyshev transform of the Padovan numbers.
 * https://oeis.org/A100049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -2*a(n-2) + a(n-3) - 2*a(n-4) - a(n-6) (n ≥ 8) Initial
 *            values: a(0)=1, a(1)=0, a(2)=-1, a(3)=1, a(4)=-1, a(5)=-3,
 *            a(6)=3, a(7)=3
 * Range:     i8 n <= 19, i16 n <= 37, i32 n <= 75, i64 n <= 151,
 *            i128 n <= 304, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;          /* pointer to array of seq_t */
    static idx_t cache_size = 0;        /* number of elements cached */

    /* initial table for first 8 terms */
    static const seq_t init[8] = {1, 0, -1, 1, -1, -3, 3, 3};

    /* allocate cache initially (first call) */
    if (cache == NULL) {
        cache_size = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL)
            return 0;  /* out of memory: caller may handle, but not expected */
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init[i];
    }

    /* extend cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (new_cache == NULL)
            return 0;  /* out of memory */
        cache = new_cache;
        /* compute new values from old_size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* recurrence: a(i) = -2*a(i-2) + a(i-3) - 2*a(i-4) - a(i-6) */
            seq_t v = -2 * cache[i - 2]      /* -2*a(i-2) */
                      + cache[i - 3]          /* + a(i-3) */
                      - 2 * cache[i - 4]      /* -2*a(i-4) */
                      - cache[i - 6];         /* - a(i-6) */
            cache[i] = v;
        }
        cache_size = new_size;
    }

    return cache[n];
}
