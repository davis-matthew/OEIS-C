/*
 * A232059  Number of n X 2 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally or vertically, with no adjacent values
 *          equal.
 * https://oeis.org/A232059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-3) for n > 3, * a(1) = 2, a(2) = 6, a(3) =
 *            14
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset starts at 1, value 0 for safety */

    /* Memoization storage */
    static seq_t *cache = NULL;      /* array of previously computed values */
    static size_t cap = 0;           /* current allocated capacity */
    static idx_t size = 0;           /* highest valid index stored */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cap = 4;                     /* capacity for indices 0..3 (0 unused) */
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) exit(1);        /* allocation failure */

        cache[0] = (seq_t)0;        /* unused, keeps index tidy */
        cache[1] = (seq_t)2;        /* a(1) */
        cache[2] = (seq_t)6;        /* a(2) */
        cache[3] = (seq_t)14;       /* a(3) */
        size = 3;
    }

    /* If requested n is already computed, return it */
    if (n <= size) return cache[n];

    /* Ensure we have sufficient capacity to store up to n */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while (newcap <= (size_t)n) newcap <<= 1;   /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (!tmp) exit(1);                          /* allocation failure */
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = size + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] * (seq_t)2;
        if (i >= 3) val += cache[i - 3];
        cache[i] = val;
    }
    size = n;
    return cache[n];
}
