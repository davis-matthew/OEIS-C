/*
 * A076725  a(n) = a(n-1)^2 + a(n-2)^4, a(0) = a(1) = 1.
 * https://oeis.org/A076725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = a(n-1)^2 + a(n-2)^4 with a(0) = 1, a(1) = 1 Recurrence
 *            implemented with memoization for any seq_t type
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>        /* malloc, realloc */

seq_t a(idx_t n) {
    /* static cache for previously computed terms */
    static seq_t *cache  = NULL;   /* array of terms */
    static size_t   cap    = 0;    /* allocated capacity */
    static idx_t    last  = 1;     /* highest index computed (initially 1) */

    /* initialise cache on first call */
    if (cap == 0) {
        cap = 2; /* enough for a(0) and a(1) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) { /* abort on allocation failure */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
    }

    /* grow cache if necessary to hold index n */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* abort on allocation failure */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        const seq_t prev1 = cache[i - 1];
        const seq_t prev2 = cache[i - 2];

        seq_t term1 = prev1 * prev1;          /* a(i-1)^2          */
        seq_t sq2   = prev2 * prev2;          /* a(i-2)^2          */
        seq_t term2 = sq2 * sq2;              /* a(i-2)^4 = (a^2)^2*/

        cache[i] = term1 + term2;
    }
    if (last < n) last = n;

    return cache[(size_t)n];
}
