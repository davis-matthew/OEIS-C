/*
 * A211626  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...-1,1,...,n} and w+4x+4y>0.
 * https://oeis.org/A211626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence: a(0) = 0, a(1) = 4, a(2) = 32, a(3) = 108, a(4) = 250,
 * a(5) = 492, a(6) = 854, a(7) = 1360, a(8) = 2021, a(9) = 2885.
 * For n > 9:
 *   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-4) - 4*a(n-5) + 2*a(n-6)
 *          - a(n-8) + 2*a(n-9) - a(n-10)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated array size */
    static size_t last  = (size_t)-1; /* last computed index */

    /* base values for n < 10 */
    static const seq_t base[10] = {
        0, 4, 32, 108, 250, 492, 854, 1360, 2021, 2885
    };

    /* If we already have the value cached, return it */
    if (last != (size_t)-1 && n <= last)
        return cache[n];

    /* Initialise cache if needed */
    if (cache == NULL) {
        cap = 16;                          /* initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (cache == NULL) return 0;      /* allocation failed silently */

        /* fill the known first 10 values */
        for (size_t i = 0; i < 10 && i < cap; ++i)
            cache[i] = base[i];
        last = 9;
    }

    /* Grow the cache if required */
    while ((size_t)n + 1 > cap) {
        size_t newcap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (tmp == NULL) return 0; /* allocation failed silently */
        cache = tmp;
        cap = newcap;
    }

    /* Compute values from last+1 up to n */
    for (idx_t i = (idx_t)(last + 1); i <= n; ++i) {
        if (i < 10) {
            cache[i] = base[i];
        } else {
            /* compute using the recurrence, avoiding
               intermediate underflow by grouping terms */
            seq_t pos = cache[i-1];
            pos += cache[i-1];                    /* 2*a(i-1) */
            pos += cache[i-4];
            pos += cache[i-4];                    /* 2*a(i-4) */
            pos += cache[i-6];
            pos += cache[i-6];                    /* 2*a(i-6) */
            pos += cache[i-9];
            pos += cache[i-9];                    /* 2*a(i-9) */

            seq_t neg = cache[i-2];
            neg += (cache[i-5] << 2);              /* 4*a(i-5) */
            neg += cache[i-8];                    /* a(i-8)  */
            neg += cache[i-10];                   /* a(i-10) */

            cache[i] = pos - neg;
        }
    }

    last = (size_t)n;
    return cache[n];
}
