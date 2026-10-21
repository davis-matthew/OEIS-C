/*
 * A206679  Number of (n+1) X 3 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having an unequal number of clockwise and counterclockwise edge
 *          increases.
 * https://oeis.org/A206679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 13*a(n-2) + 11*a(n-3) + 12*a(n-4) + 6*a(n-5)
 *            for n>6 Initial values: a(1) = 306 a(2) = 1728 a(3) = 9792 a(4)
 *            = 55836 a(5) = 317736 a(6) = 1809324
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute OEIS sequence A206679 */
seq_t a(idx_t n)
{
    /* static cache of computed values, 1‑based indexing */
    static seq_t *cache = NULL;     /* pointer to array of seq_t */
    static idx_t cap   = 0;         /* allocated capacity (>= n+1) */
    static idx_t last  = 0;         /* highest index computed so far */

    /* initialise cache with the first six terms */
    if (cache == NULL) {
        cap = 16;   /* initial capacity, will grow as needed */
        cache = (seq_t*)malloc(sizeof(seq_t) * cap);
        if (cache == NULL) return 0; /* out of memory: return 0 as fallback */

        /* set dummy at index 0 to avoid out‑of‑range when n<1 */
        cache[0] = 0;

        cache[1] = 306;
        cache[2] = 1728;
        cache[3] = 9792;
        cache[4] = 55836;
        cache[5] = 317736;
        cache[6] = 1809324;
        last = 6;
    }

    /* extend cache if requested index exceeds current capacity */
    if (n >= cap) {
        idx_t newcap = cap * 2;
        if (newcap <= n) newcap = n * 2 + 1;   /* at least enough for n */
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * newcap);
        if (tmp == NULL) return 0; /* out of memory: fallback */
        cache = tmp;
        cap = newcap;
    }

    /* compute missing values up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = 3 * cache[i - 1]
                 + 13 * cache[i - 2]
                 + 11 * cache[i - 3]
                 + 12 * cache[i - 4]
                 + 6  * cache[i - 5];
    }
    if (n > last) last = n;

    /* for the offset of 1, return 0 for n==0 which should not happen */
    return (n == 0) ? 0 : cache[n];
}
