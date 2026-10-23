/*
 * A041757  Denominators of continued fraction convergents to sqrt(398).
 * https://oeis.org/A041757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 798*a(n-4) - a(n-8) for n > 7
   with initial values: a(0)=1, a(1)=1, a(2)=19, a(3)=20,
   a(4)=779, a(5)=799, a(6)=15161, a(7)=15960 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;   /* allocated capacity */
    static size_t known   = 0;       /* number of computed terms */

    /* initialise cache with the eight known terms */
    if (cache_size == 0) {
        /* allocate at least 8 slots */
        cache_size = 8;
        if ((size_t)n + 1 > cache_size)
            cache_size = (size_t)n + 1;  /* minimal size for this n */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;           /* malloc failed */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)19;
        cache[3] = (seq_t)20;
        cache[4] = (seq_t)779;
        cache[5] = (seq_t)799;
        cache[6] = (seq_t)15161;
        cache[7] = (seq_t)15960;
        known = 8;                      /* terms 0..7 are ready */
    }

    /* enlarge cache if required */
    if ((size_t)n + 1 > cache_size) {
        size_t newsize = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) return 0;             /* realloc failed */
        cache     = tmp;
        cache_size = newsize;
        /* no new terms computed yet beyond `known` */
    }

    /* if the requested term is already known, return it */
    if ((size_t)n < known)
        return cache[(size_t)n];

    /* compute missing terms up to n */
    for (size_t i = known; i <= (size_t)n; ++i) {
        if (i < 8)
            continue;                   /* initial terms already set */
        cache[i] = (seq_t)798 * cache[i-4] - cache[i-8];
    }
    known = (size_t)n + 1;               /* update known count */

    return cache[(size_t)n];
}
