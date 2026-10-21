/*
 * A094554  Number of closed walks of length n at a base vertex of a truncated
 *          tetrahedron (triangular prism).
 * https://oeis.org/A094554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=0, a(2)=3, a(3)=2 a(n) = 2*a(n-1) + 5*a(n-2) -
 *            6*a(n-3) for n ≥ 4
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence follows the closed form a(n) = (3^n + 2*(-2)^n + 1)/6 for n>0 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache == NULL) {
        /* Allocate space for the first term (index 0) */
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Expand the cache if needed */
    if (n >= cache_len) {
        /* Allocate more space: new_len = n+1  (to include index n) */
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failure – halt (no restoration needed for this exercise) */
            exit(1);
        }
        /* Compute new terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 1) {
                new_cache[i] = (seq_t)0;
            } else if (i == 2) {
                new_cache[i] = (seq_t)3;
            } else if (i == 3) {
                new_cache[i] = (seq_t)2;
            } else {
                /* Recurrence: a(i) = 2*a(i-1) + 5*a(i-2) - 6*a(i-3) */
                seq_t term1 = (seq_t)2 * new_cache[i - 1];
                seq_t term2 = (seq_t)5 * new_cache[i - 2];
                seq_t term3 = (seq_t)6 * new_cache[i - 3];
                new_cache[i] = term1 + term2 - term3;
            }
        }
        cache = new_cache;
        cache_len = new_len;
    }

    return cache[n];
}
