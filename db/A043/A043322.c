/*
 * A043322  Numbers having two 0's in base 3.
 * https://oeis.org/A043322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer that has exactly two zeros in its base‑3
 *            representation
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache  = NULL;
    static size_t  cap   = 0;
    static size_t  len   = 0;
    static seq_t  next   = 9;      /* first integer having two zeros in base 3 */

    /* lazy allocation of cache */
    if (cache == NULL) {
        cap   = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)   /* allocation failure – give up */
            return (seq_t)0;
    }

    /* enlarge cache if necessary */
    if (len >= cap) {
        size_t new_cap = cap * 2;
        seq_t *tmp     = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            cache = NULL;
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* generate terms until we have at least n in the cache */
    while (len < n) {
        seq_t x = next;
        next = next + (seq_t)1;               /* advance candidate */

        /* count zeros in base‑3 representation of x */
        seq_t t      = x;
        int   zeros  = 0;

        while (t > (seq_t)0) {
            if ((t % (seq_t)3) == (seq_t)0)
                zeros++;
            t = t / (seq_t)3;
        }

        if (zeros == 2) {
            cache[len++] = x;
        }
    }

    return cache[n - 1];
}
