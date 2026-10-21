/*
 * A099254  Self-convolution of A010892. The g.f. is 1/(Alexander polynomial
 *          of granny knot).
 * https://oeis.org/A099254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=1, a(3)=-2; a(n) = 2*a(n-1) - 3*a(n-2) +
 *            2*a(n-3) - a(n-4) for n >= 4
 * Range:     i8 n <= 192, i16 n <= 9599, i32 n <= 9599, i64 n <= 9599,
 *            i128 n <= 9599, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n < 4) {
        switch (n) {
            case 0: return 1;
            case 1: return 2;
            case 2: return 1;
            case 3: return -2;
            /* no default needed */
        }
    }

    /* Lazy static cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* number of elements stored (>=1) */

    /* Desired number of elements */
    size_t need = (size_t)n + 1;

    if (cap == 0) {
        /* First allocation: at least 4 elements */
        cap = (need > 4) ? need : 4;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */
        /* initialise the first four terms */
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 1;
        cache[3] = -2;
        /* compute any remaining terms up to cap-1 */
        for (idx_t i = 4; i < (idx_t)cap; ++i) {
            cache[i] = 2 * cache[i-1] - 3 * cache[i-2]
                     + 2 * cache[i-3] - cache[i-4];
        }
    } else if (cap < need) {
        /* grow cache to required size */
        size_t newcap = cap * 2;
        if (newcap < need) newcap = need;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        cache = tmp;
        /* compute missing terms */
        for (idx_t i = (idx_t)cap; i < (idx_t)newcap; ++i) {
            cache[i] = 2 * cache[i-1] - 3 * cache[i-2]
                     + 2 * cache[i-3] - cache[i-4];
        }
        cap = newcap;
    }

    return cache[(size_t)n];
}
