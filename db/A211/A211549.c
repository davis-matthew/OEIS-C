/*
 * A211549  Number of (n+1) X (n+1) -9..9 symmetric matrices with every 2 X 2
 *          subblock having sum zero and one or two distinct values.
 * https://oeis.org/A211549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=31, a(2)=43, a(3)=61, a(n)=2*a(n‑1)-a(n‑3) for n≥4
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated size (highest index that may exist) */
    static idx_t  curr  = 0;      /* highest index that has been computed */

    if (n == 0) return 0; /* undefined for offset 1, but safe guard */

    /* Initialise the cache on first call */
    if (cache == NULL) {
        cap = 4;                     /* allocate space for indices 0..3 (0 unused) */
        cache = (seq_t *)malloc(sizeof(seq_t) * (cap + 1));
        if (!cache) return 0;        /* allocation failed */
        /* base values */
        cache[1] = (seq_t)31;
        cache[2] = (seq_t)43;
        cache[3] = (seq_t)61;
        curr = 3;
    }

    /* Expand allocated space if needed */
    if ((size_t)n > cap) {
        size_t newcap = cap;
        while (newcap < (size_t)n) newcap = (newcap == 0) ? 4 : newcap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (newcap + 1));
        if (!tmp) return 0;         /* reallocation failed */
        cache = tmp;
        cap = newcap;
    }

    /* Compute terms up to n if not already computed */
    while (curr < n) {
        /* a(i) = 2*a(i-1) - a(i-3) */
        seq_t val = cache[curr] + cache[curr]; /* 2*a(i-1) */
        val = val - cache[curr - 2];           /* subtract a(i-3) */
        cache[curr + 1] = val;
        ++curr;
    }

    return cache[n];
}
