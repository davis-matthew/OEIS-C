/*
 * A091002  Number of walks of length n between non-adjacent nodes on the
 *          Petersen graph.
 * https://oeis.org/A091002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3^(n+1) - (-2)^(n+1) - 5)/30
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 0, a(2) = 1;
   for n >= 3: a(n) = 2*a(n-1) + 5*a(n-2) - 6*a(n-3) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the small indices directly */
    if (n == 0 || n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;

    /* Static cache to support arbitrary call order */
    static seq_t *cache = NULL;
    static idx_t cap    = 0;

    /* Extend the cache if needed */
    if (n >= cap) {
        idx_t newcap = n + 1;                         /* store up to index n */
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!cache)                                   /* allocation failure */
            return (seq_t)0;

        if (cap == 0) {                               /* first allocation */
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)1;

            for (idx_t i = 3; i < newcap; ++i)
                cache[i] = (seq_t)2 * cache[i - 1]
                          + (seq_t)5 * cache[i - 2]
                          - (seq_t)6 * cache[i - 3];
        } else {                                       /* extend existing cache */
            for (idx_t i = cap; i < newcap; ++i)
                cache[i] = (seq_t)2 * cache[i - 1]
                          + (seq_t)5 * cache[i - 2]
                          - (seq_t)6 * cache[i - 3];
        }
        cap = newcap;
    }

    return cache[n];
}
