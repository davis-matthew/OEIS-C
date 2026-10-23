/*
 * A192816  a(n) = A192815(n)/2.
 * https://oeis.org/A192816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) + 7*a(n-3) Initial values: a(0) = 0,
 *            a(1) = 1, a(2) = 2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Cached values of the sequence.  The array grows as
       larger indices are requested. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of valid entries */

    /* Remember previous length before possibly resizing */
    size_t old_len = cache_len;

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            ((size_t)n + 1) * sizeof(seq_t));
        if (new_cache == NULL)          /* allocation failure: fall back */
            new_cache = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
        cache = new_cache;
        /* Initialise any newly allocated entries to zero */
        for (size_t i = old_len; i < (size_t)n + 1; ++i)
            cache[i] = (seq_t)0;
        cache_len = (size_t)n + 1;
    }

    /* Compute any terms that have not yet been computed */
    for (size_t k = old_len; k <= (size_t)n; ++k) {
        if (k == 0)
            cache[k] = (seq_t)0;
        else if (k == 1)
            cache[k] = (seq_t)1;
        else if (k == 2)
            cache[k] = (seq_t)2;
        else {
            seq_t v = (seq_t)5 * cache[k - 1];
            v -= (seq_t)3 * cache[k - 2];
            v += (seq_t)7 * cache[k - 3];
            cache[k] = v;
        }
    }

    return cache[(size_t)n];
}
