/*
 * A123954  Expansion of g.f.: x^5/((1-3*x)*(1-4*x)*(1-5*x)*(1 - 8*x +
 *          13*x^2)).
 * https://oeis.org/A123954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) -156*a(n-2) +592*a(n-3) -1091*a(n-4)
 *            +780*a(n-5), initial values: a(1)=0, a(2)=0, a(3)=0, a(4)=0,
 *            a(5)=1
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 15, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* trivial cases before the offset */
    if (n <= 5) {
        static const seq_t init[5] = {0, 0, 0, 0, 1};
        return init[n - 1];
    }

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* current size of cache */
    static idx_t  last = 0;      /* highest n computed so far */

    /* initialise static cache if first call */
    if (cache == NULL) {
        /* start with room for the first five terms */
        seq_t *c = seq_alloc(5);
        if (c == NULL) return 0;          /* out-of-memory – abort */
        c[0] = 0; c[1] = 0; c[2] = 0; c[3] = 0; c[4] = 1;
        cache = c;
        cap   = 5;
        last  = 5;
    }

    /* enlarge cache if needed */
    if ((size_t)n > cap) {
        seq_t *c = seq_realloc(cache, cap, (size_t)n);
        if (c == NULL) return 0;          /* out-of-memory – abort */
        cache = c;
        cap   = (size_t)n;
    }

    /* compute missing terms up to n, if any */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t t = 20 * cache[i - 2]             /* a(i-1) */
                -156 * cache[i - 3]             /* a(i-2) */
                +592 * cache[i - 4]             /* a(i-3) */
                -1091 * cache[i - 5]            /* a(i-4) */
                +780 * cache[i - 6];            /* a(i-5) */
        cache[i - 1] = t;
    }
    last = n;

    return cache[n - 1];
}
