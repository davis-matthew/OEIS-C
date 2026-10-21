/*
 * A218985  Power ceiling sequence of 2+sqrt(6).
 * https://oeis.org/A218985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A218985
   a(n) = 5*a(n-1) - 2*a(n-2) - 2*a(n-3),
   a(0) = 5, a(1) = 23, a(2) = 103 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* number of cached terms currently stored */

    /* enlarge cache if required */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;                          /* need at least n+1 terms */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                               /* allocation failed */
        cache = tmp;

        /* compute new terms */
        for (idx_t i = cache_cap; i < new_cap; ++i) {
            if (i == 0) {
                cache[i] = 5;                            /* a(0) */
            } else if (i == 1) {
                cache[i] = 23;                           /* a(1) */
            } else if (i == 2) {
                cache[i] = 103;                          /* a(2) */
            } else {
                /* recurrence: a(n) = 5*a(n-1) - 2*a(n-2) - 2*a(n-3) */
                cache[i] = 5 * cache[i - 1]
                           - 2 * cache[i - 2]
                           - 2 * cache[i - 3];
            }
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
