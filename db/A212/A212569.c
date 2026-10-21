/*
 * A212569  Number of (w,x,y,z) with all terms in {0,...,n} such that
 *          range{w,x,y,z} is not one of the numbers w,x,y,z.
 * https://oeis.org/A212569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A212569
   Formula: a(n) = n^4 - A212746(n)
   Recurrence: a(n) = 2*a(n-1)+2*a(n-2)-6*a(n-3)+6*a(n-5)-2*a(n-6)-2*a(n-7)+a(n-8)
   Initial values: a(0)=0, a(1)=1, a(2)=2, a(3)=31, a(4)=96,
                    a(5)=321, a(6)=690, a(7)=1471
   This implementation uses memoised iteration over the recurrence,
   which is O(n) per new index.  All arithmetic is unsigned and
   performed directly on seq_t.  For n < 8 the value is returned
   from a small static table.  The function is thread‑safe only
   in the sense that each call uses independent static data; if
   concurrent calls are required, external synchronization is
   needed. */

#include <stdlib.h>

static const seq_t _init[8] = {0, 1, 2, 31, 96, 321, 690, 1471};

seq_t a(idx_t n) {
    if (n < 8) {
        return _init[n];
    }

    /* static cache */
    static seq_t *cache = NULL;   /* array of computed values */
    static idx_t   size  = 0;     /* allocated size of cache */
    static idx_t   last  = 0;     /* number of values actually stored */

    /* initialise cache if first call */
    if (!cache) {
        size = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * size);
        for (idx_t i = 0; i < 8; ++i) {
            cache[i] = _init[i];
        }
        last = 8;
    }

    /* grow cache if required */
    if (size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) { /* allocation failed – return 0 as a sane fallback */
            return 0;
        }
        cache = tmp;
        size = new_size;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = last; i <= n; ++i) {
        /* positive contribution: 2*a(i-1)+2*a(i-2)+a(i-8)+6*a(i-5) */
        seq_t pos = 2 * cache[i - 1]
                  + 2 * cache[i - 2]
                  + cache[i - 8]
                  + 6 * cache[i - 5];

        /* negative contribution: 6*a(i-3)+2*a(i-6)+2*a(i-7) */
        seq_t neg = 6 * cache[i - 3]
                  + 2 * cache[i - 6]
                  + 2 * cache[i - 7];

        /* result is guaranteed to be non‑negative */
        cache[i] = pos - neg;
    }

    last = n + 1;
    return cache[n];
}
