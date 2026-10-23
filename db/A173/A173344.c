/*
 * A173344  a(n+4) = a(n+3) - 2*a(n+2) - a(n+1) - a(n), starting with (0, 1,
 *          0, -2).
 * https://oeis.org/A173344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+4) = a(n+3) - 2*a(n+2) - a(n+1) - a(n),
   with initial values a(0)=0, a(1)=1, a(2)=0, a(3)=-2. */

#include <stdlib.h>
#ifdef __cplusplus
# include <new>
#endif

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t  cap     = 0;
    static idx_t  computed = 0;

    /* Initialise cache with the four starting values on first call. */
    if (!cache) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
#ifdef __cplusplus
        for (idx_t i = 0; i < cap; ++i)
            new (cache + i) seq_t();
#endif
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)(-2);
        computed = 4;
    }

    /* Grow the cache if the requested index exceeds current capacity. */
    if (n >= cap) {
        idx_t new_cap = cap ? cap * 2 : 4;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cap   = new_cap;
#ifdef __cplusplus
        for (idx_t i = computed; i < cap; ++i)
            new (cache + i) seq_t();
#endif
    }

    /* Compute all missing terms up to n using the recurrence. */
    if (n >= computed) {
        for (idx_t i = computed; i <= n; ++i) {
            cache[i] = cache[i - 1]
                     - (seq_t)2 * cache[i - 2]
                     - cache[i - 3]
                     - cache[i - 4];
        }
        computed = n + 1;
    }

    return cache[n];
}
