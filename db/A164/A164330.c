/*
 * A164330  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A164330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A164330: Number of reduced words of length n in a Coxeter group
 * on 45 generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 *
 * Recurrence (for n >= 7):
 *   a(n) = -946 * a(n-6) + 43 * (a(n-1) + a(n-2) + a(n-3) + a(n-4) + a(n-5))
 *
 * Initial terms:
 *   a(0) = 1                      a(6) = 7421229090
 *   a(1) = 45                     a(7) = 326534036400
 *   a(2) = 1980                   a(8) = 14367495685950
 *   a(3) = 87120                  a(9) = 632169725893200
 *   a(4) = 3833280                a(10) = 27815464230602400
 *   a(5) = 168664320
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size of cache */
    static size_t last  = 0;   /* highest index already computed */

    /* first initialisation on first call */
    if (cap == 0) {
        /* preallocate some space; will grow if needed */
        cap   = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure (unlikely in tests) */

        /* initialise known first seven terms */
        cache[0] = 1;
        cache[1] = 45;
        cache[2] = 1980;
        cache[3] = 87120;
        cache[4] = 3833280;
        cache[5] = 168664320;
        cache[6] = 7421229090;

        last = 6;
    }

    /* if the requested term is already cached, return it */
    if ((size_t)n <= last) {
        return cache[(size_t)n];
    }

    /* otherwise grow the cache to hold up to n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n (recurrence starts at 7) */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        /* sum a(i-1) … a(i-5) */
        seq_t sum5 = cache[i-1];
        sum5 += cache[i-2];
        sum5 += cache[i-3];
        sum5 += cache[i-4];
        sum5 += cache[i-5];

        /* a(i) = -946*a(i-6) + 43*sum5 */
        seq_t term = 43 * sum5;
        term -= 946 * cache[i-6];

        cache[i] = term;
    }

    last = (size_t)n;
    return cache[(size_t)n];
}
