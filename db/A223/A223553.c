/*
 * A223553  Petersen graph (3,1) coloring a rectangular array: number of n X 5
 *          0..5 arrays where 0..5 label nodes of a graph with edges 0,1 0,3
 *          3,5 3,4 1,2 1,4 4,5 2,0 2,5 and every array movement to a
 *          horizontal or antidiagonal neighbor moves along an edge of this
 *          graph, with the array starting at 0.
 * https://oeis.org/A223553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 95*a(n-1) - 626*a(n-2) + 720*a(n-3) for n > 4 with
 *            a(1)=81, a(2)=6939, a(3)=609309, a(4)=53599905. This uses the
 *            empirically found linear recurrence. The implementation memoises
 *            computed terms so that calls in arbitrary order are efficient
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *cache  = NULL;      /* array of computed values (index 1..) */
    static size_t  cap   = 0;         /* current capacity of 'cache'          */
    static idx_t   last  = 4;         /* largest index that has been computed */

    /* first call – allocate & initialise */
    if (cache == NULL) {
        /* start with a modest size, will grow if needed */
        cap = 20;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – return 0 */
        /* initialise all entries to 0 */
        for (size_t i = 0; i < cap; ++i) cache[i] = 0;
        /* provide initial values */
        cache[1] = (seq_t)81;
        cache[2] = (seq_t)6939;
        cache[3] = (seq_t)609309;
        cache[4] = (seq_t)53599905;
    }

    if (n == 0) return 0;              /* (offset 1 – undefined below 1) */

    /* enlarge the cache if we need more space */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap) newcap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure – return 0 */
        /* initialise new slots to 0 */
        for (size_t i = cap; i < newcap; ++i) tmp[i] = 0;
        cache = tmp;
        cap   = newcap;
    }

    /* if everything needed already computed, great */
    if (n <= last) return cache[n];

    /* compute terms up to 'n' (only the missing ones) */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t t1 = cache[k-1] * (seq_t)95;
        seq_t t2 = cache[k-3] * (seq_t)720;
        seq_t sum = t1 + t2;
        seq_t t3 = cache[k-2] * (seq_t)626;
        cache[k] = sum - t3;          /* recurrence: sum - term3 */
    }
    last = n;
    return cache[n];
}
