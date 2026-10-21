/*
 * A206278  Total number of triangles in Cayley graph Cay(Z_{2^n}, QR*(2^n)).
 * https://oeis.org/A206278
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1)+20*a(n-2)-160*a(n-3)-64*a(n-4)+512*a(n-5) for
 *            n>=8 Initial values: a(3)=0, a(4)=0, a(5)=128, a(6)=1024,
 *            a(7)=6656
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 24, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    /* static cache (dynamic a(n)) */
    static seq_t *cache = NULL;
    static size_t  cap    = 0;      /* capacity of cache array            */
    static idx_t   last   = 2;      /* highest index computed already      */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 8; /* indices 0..7 inclusive */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* values 0..2 are 0 by definition */
        cache[0] = cache[1] = cache[2] = (seq_t)0;
        /* initial terms */
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)0;
        cache[5] = (seq_t)128;
        cache[6] = (seq_t)1024;
        cache[7] = (seq_t)6656;
        last = 7;
    }

    /* expand cache if necessary */
    if ((size_t)(n + 1) > cap) {
        while ((size_t)(n + 1) > cap)
            cap <<= 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* compute missing terms up to n */
    for (idx_t k = last + 1; k <= n; ++k) {
        /* recurrence valid for k >= 8 */
        if (k <= 7) { /* values already initialised */
            continue;
        }
        seq_t a1 = cache[k - 1];
        seq_t a2 = cache[k - 2];
        seq_t a3 = cache[k - 3];
        seq_t a4 = cache[k - 4];
        seq_t a5 = cache[k - 5];

        seq_t pos = (seq_t)8  * a1 + (seq_t)20 * a2 + (seq_t)512 * a5;
        seq_t neg = (seq_t)160 * a3 + (seq_t)64 * a4;
        cache[k] = pos - neg; /* pos >= neg for all n where a(n) is defined */
    }

    last = n;
    return cache[n];
}
