/*
 * A198287  Number of 2n X 2 0..3 arrays with values 0..3 introduced in row
 *          major order and each element equal to exactly one horizontal and
 *          vertical neighbor.
 * https://oeis.org/A198287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A198287
   Formula: a(n) = 23*a(n-1) - 103*a(n-2) + 81*a(n-3),
   with a(1)=2, a(2)=17, a(3)=272.  All terms are positive.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)                /* offset is 1 – undefined below 1 */
        return 0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;   /* number of stored terms */

    /* Grow cache if needed */
    if (n > cap) {
        idx_t   new_cap   = n;
        seq_t *new_cache  = (seq_t *)realloc(cache,
                                              new_cap * sizeof(seq_t));
        if (!new_cache)        /* allocation failure – return 0 */
            return 0;

        /* Initialise new entries */
        for (idx_t i = cap; i < new_cap; ++i) {
            if (i == 0)                      /* a(1) */
                new_cache[i] = (seq_t)2;
            else if (i == 1)                 /* a(2) */
                new_cache[i] = (seq_t)17;
            else if (i == 2)                 /* a(3) */
                new_cache[i] = (seq_t)272;
            else {                           /* recurrence  */
                seq_t v  = (seq_t)23 * new_cache[i-1];
                v       += (seq_t)81 * new_cache[i-3];
                v       -= (seq_t)103 * new_cache[i-2];
                new_cache[i] = v;
            }
        }
        cache = new_cache;
        cap   = new_cap;
    }

    return cache[n-1];
}
