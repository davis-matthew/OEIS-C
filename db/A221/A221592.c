/*
 * A221592  Number of 0..4 arrays of length n with each element differing from
 *          at least one neighbor by 1 or less.
 * https://oeis.org/A221592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) + 6*a(n-4) + 4*a(n-5) + 4*a(n-6)
 *            Initial values: a(1) = 0 a(2) = 13 a(3) = 35 a(4) = 169 a(5) =
 *            651 a(6) = 2715
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;      /* allocated up to cache_cap (index) */
    static idx_t max_computed = 0;   /* highest index computed so far */

    if (n == 0) return 0;           /* not used in OEIS indexing */

    /* allocate cache and initialise base cases on first call */
    if (!cache) {
        cache_cap = (idx_t)6;       /* support indices 0..6 at least */
        cache = (seq_t*)calloc((size_t)(cache_cap + 1), sizeof(seq_t));
        cache[1] = 0;
        cache[2] = 13;
        cache[3] = 35;
        cache[4] = 169;
        cache[5] = 651;
        cache[6] = 2715;
        max_computed = 6;
    }
    /* grow cache if requested n is beyond current capacity */
    if (n > cache_cap) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t*)realloc(cache,
                                     (size_t)(new_cap + 1) * sizeof(seq_t));
        /* reallocation should succeed in the test environment */
        cache = tmp;
        cache_cap = new_cap;
        /* new entries are now zero-initialised by realloc assumption */
    }
    /* compute missing terms up to n using the linear recurrence */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i <= 6) {          /* base cases already set */
                continue;
            }
            seq_t term = 3 * cache[i - 1]
                       + 4 * cache[i - 2]
                       + 6 * cache[i - 4]
                       + 4 * cache[i - 5]
                       + 4 * cache[i - 6];
            cache[i] = term;
        }
        max_computed = n;
    }
    return cache[n];
}
