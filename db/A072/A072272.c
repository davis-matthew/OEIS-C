/*
 * A072272  Number of active cells in n-th stage of growth of two-dimensional
 *          cellular automaton defined by "Rule 614", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A072272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(0)=1; thereafter a(2t) = a(t), a(4t+1) = 5*a(t), a(4t+3) =
 *            3*a(2t+1)+2*a(t)
 * Range:     u8 n <= 22, u16 n <= 510, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of computed values; 0 will serve as "not computed"
       since the sequence contains only positive integers. */
    static seq_t *cache = NULL;
    static size_t capacity = 0;

    size_t idx = (size_t)n;
    if (idx >= capacity) {
        /* grow the cache to at least idx+1 */
        size_t newcap = capacity ? capacity : 64;
        while (newcap <= idx)
            newcap <<= 1;

        /* realloc (cast for C++ compatibility) */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – compute without caching */
            if (n == 0)
                return 1;
            if ((n & 1) == 0)
                return a(n >> 1);
            if ((n & 3) == 1)
                return 5 * a((n - 1) >> 2);
            idx_t t = (n - 3) >> 2;
            return 3 * a((t << 1) + 1) + 2 * a(t);
        }

        /* zero the new part of the array */
        if (cache == NULL) {
            memset(tmp, 0, newcap * sizeof(seq_t));
        } else {
            memset(tmp + capacity, 0, (newcap - capacity) * sizeof(seq_t));
        }

        cache = tmp;
        capacity = newcap;
    }

    /* Return cached value if available */
    if (cache[idx] != 0)
        return cache[idx];

    /* compute value recursively */
    seq_t result;
    if (n == 0) {
        result = 1;
    } else if ((n & 1) == 0) {   /* n = 2t */
        result = a(n >> 1);
    } else {
        if ((n & 3) == 1) {       /* n = 4t+1 */
            result = 5 * a((n - 1) >> 2);
        } else {                  /* n = 4t+3 */
            idx_t t = (n - 3) >> 2;
            seq_t a2t1 = a((t << 1) + 1);
            seq_t at   = a(t);
            result = 3 * a2t1 + 2 * at;
        }
    }

    /* store and return */
    cache[idx] = result;
    return result;
}
