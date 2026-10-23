/*
 * A132353  a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4), starting with 1, 2, 6, 20.
 * https://oeis.org/A132353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4), a(0)=1, a(1)=2, a(2)=6,
 *            a(3)=20
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* allocated size in elements */
    static idx_t    nth   = 0;   /* number of terms currently stored (max index+1) */

    /* Return the first four terms directly */
    if (n < 4) {
        switch ((idx_t)n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)2;
            case 2: return (seq_t)6;
            case 3: return (seq_t)20;
        }
    }

    /* Initialise the cache on first use */
    if (cap == 0) {
        cap = 8;                      /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)20;
        nth    = 4;
    }

    /* If the requested term is already available, return it */
    if (n < nth)
        return cache[n];

    /* Grow the cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = nth; i <= n; ++i) {
        seq_t val = (seq_t)3 * cache[i - 1] - cache[i - 3] + (seq_t)3 * cache[i - 4];
        cache[i] = val;
    }
    nth = n + 1;            /* we now have terms up to n */

    return cache[n];
}
