/*
 * A123672  a(1) = 1; for n > 1, a(n) = (2^n-1)*a(n-1) + (-1)^n.
 * https://oeis.org/A123672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n − 1) * a(n−1) + (−1)^n , a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                       /* for realloc, malloc, free */

/*  pow2(idx_t exp) is provided in ../utils/pow2.h;  we use it to
    generate 2^i on the fly.  The function is available for all
    supported builds. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /*  Values for n < 1 are undefined by the OEIS definition.
        Return 0 in that case to keep the function total.  */
    if (n < 1)
        return 0;

    static seq_t *cache = NULL;   /* cache[1] holds a(1) */
    static size_t   cap   = 0;    /* allocated size of cache (+1 for 0) */
    static idx_t    last  = 0;    /* last index computed and stored */

    /*  Grow cache if necessary  */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)              /* allocation failed */
            return 0;
        cache = tmp;
        cap   = new_cap;
    }

    /*  Initialise base value on first call  */
    if (last == 0) {
        cache[1] = 1;
        last = 1;
    }

    /*  Compute any missing values up to n  */
    for (idx_t i = last + 1; i <= n; ++i) {
        /*  Compute (2^i – 1)  */
        seq_t factor = pow2(i);   /* exact power of 2 in seq_t */
        factor -= 1;              /* 2^i – 1 */

        /*  a(i) = factor * a(i-1) + (-1)^i  */
        seq_t val = factor * cache[i - 1];
        if ((i & 1) == 0)        /*  i even: (-1)^i = +1  */
            val += 1;
        else                     /*  i odd:  (-1)^i = –1  */
            val -= 1;

        cache[i] = val;
    }

    last = n;
    return cache[n];
}
