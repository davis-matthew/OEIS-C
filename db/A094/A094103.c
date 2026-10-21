/*
 * A094103  a(n) = sum along n-th diagonal of A094102 (sloping downward to
 *          left).
 * https://oeis.org/A094103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4) − a(n-5) − a(n-7) with a(1)=1,
 *            a(2)=1, a(3)=2, a(4)=3, a(5)=4, a(6)=5, a(7)=8 (offset 1). The
 *            function memoises previously‑computed values in a static array.
 *            The cache is size‑grown as needed
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 90, u64 n <= 182,
 *            u128 n <= 366, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: 1‑based indexing; cache[1] = a(1), …, etc. */
    static seq_t *cache     = NULL;
    static idx_t capacity   = 0;      /* allocated size (number of slots) */
    static idx_t last_computed = 0;   /* largest n for which cache[n] is valid */

    if (n == 0)       /* behaviour for indices below the offset is unspecified */
        return 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        capacity = 16;                  /* initial capacity */
        cache = (seq_t*)realloc(cache, capacity * sizeof(seq_t));
        if (cache == NULL) return 0;    /* out of memory – return 0 */

        /* base values, 1‑based */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)3;
        cache[5] = (seq_t)4;
        cache[6] = (seq_t)5;
        cache[7] = (seq_t)8;
        last_computed = 7;
    }

    /* grow the cache if the requested index is beyond the current capacity */
    if (n >= capacity) {
        idx_t newcap = capacity;
        while (newcap <= n)
            newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return 0;     /* allocation failed */
        cache = tmp;
        capacity = newcap;
    }

    /* compute missing terms up to n (if any) */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 2] + cache[i - 3] + cache[i - 4];
        val -= cache[i - 5];
        val -= cache[i - 7];
        cache[i] = val;
    }
    if (n > last_computed)
        last_computed = n;

    return cache[n];
}
