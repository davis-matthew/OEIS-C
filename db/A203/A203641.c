/*
 * A203641  Number of arrays of n 0..10 integers with new values introduced in
 *          order 0..10 but otherwise unconstrained.
 * https://oeis.org/A203641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..11} stirling2(n,k) (sum of Stirling numbers of
 *            the second kind)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the unofficial offset: a(1) is first term */
    if (n == 0) return 0;

    /* Static state */
    static seq_t *cache      = NULL;   /* array holding a(k) for 1 <= k <= max_comp */
    static idx_t  max_comp   = 0;      /* highest n that has been computed */

    /* Arrays for Stirling numbers S(n,k).  k ranges 1..11 (index 0 unused). */
    static seq_t prev[12] = {0};
    static seq_t curr[12] = {0};

    /* On first call, initialise storage for n=1 */
    if (max_comp == 0) {
        /* Allocate space for n=1 */
        cache = (seq_t *)calloc(2, sizeof(seq_t));   /* indices 0 and 1 */
        if (!cache) return 0;                       /* out of memory, return 0 */
        prev[1] = 1;                                /* S(1,1) = 1 */
        cache[1] = 1;                               /* a(1) = 1 */
        max_comp = 1;
    }

    /* Expand cache and compute new values up to requested n */
    if (n > max_comp) {
        /* Reallocate cache to hold up to n (index 0 unused) */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure -> 0 */
        cache = new_cache;
        /* Initialise new cache entries to 0 */
        for (idx_t i = max_comp + 1; i <= n; ++i) cache[i] = 0;

        /* Compute a(i) for i = max_comp+1 .. n */
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            /* S(i,1) = 1 by definition */
            curr[1] = 1;

            /* Compute S(i,k) for k=2..11 via recurrence */
            for (idx_t k = 2; k <= 11; ++k) {
                curr[k] = (seq_t)k * prev[k] + prev[k - 1];
            }

            /* Sum over k=1..11 to get a(i) */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 11; ++k) sum += curr[k];
            cache[i] = sum;

            /* Prepare for next iteration */
            for (idx_t k = 1; k <= 11; ++k) prev[k] = curr[k];
        }
        max_comp = n;
    }

    return cache[n];
}
