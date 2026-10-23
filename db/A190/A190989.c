/*
 * A190989  a(n) = 10*a(n-1) - 7*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 7*a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;      /* array of computed terms            */
    static size_t cap   = 0;         /* allocated capacity in elements      */
    static idx_t  max_i = 0;         /* highest index that has been computed */

    /* Allocate cache if not yet done */
    if (cache == NULL) {
        cap = (n >= 1 ? n + 1 : 1);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();

        /* Initialise base values */
        cache[0] = 0;
        if (cap > 1) {
            cache[1] = 1;
            max_i    = 1;
        } else {
            max_i    = 0;
        }
    }
    /* Grow cache to cover index n, if necessary */
    else if (n + 1 > cap) {
        size_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;

        /* Ensure base values are defined in the enlarged cache */
        cache[0] = 0;
        if (cap > 1) {
            cache[1] = 1;
            if (max_i < 1)
                max_i = 1;
        }
    }

    /* Compute any missing terms up to n */
    if (n > max_i) {
        for (idx_t i = max_i + 1; i <= n; ++i) {
            cache[i] = (seq_t)10 * cache[i - 1] - (seq_t)7 * cache[i - 2];
        }
        max_i = n;
    }

    return cache[n];
}
