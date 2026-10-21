/*
 * A084242  Least k, 1 <= k <= n, such that the number of elements of the
 *          continued fraction for n/k is maximum.
 * https://oeis.org/A084242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least k (1 <= k <= n) such that the number of terms
 *            in the continued fraction of n/k is maximum
 * Range:     u8 n <= 384, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include only standard headers that can be used in both C11 and C++17 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ----------------------------------------------------------------------------
       Static cache: a_cache[i] stores a(i) for i >= 1.
       Cache grows as needed so that repeated calls are fast.
       ---------------------------------------------------------------------------*/
    static seq_t *a_cache = NULL;      /* 1-indexed array */
    static idx_t cache_size = 0;       /* current size of the cache (max n cached) */

    /* -------------------------------------------------------------------------
       Grow cache if necessary and compute missing terms.
       -------------------------------------------------------------------------*/
    if (n > cache_size) {
        idx_t new_size = n;
        /* Allocate new memory block */
        seq_t *new_cache = (seq_t *)malloc((new_size + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – return 0 (should not happen in test harness) */
            return (seq_t)0;
        }

        /* Copy existing values if any */
        if (cache_size > 0) {
            for (idx_t i = 1; i <= cache_size; ++i)
                new_cache[i] = a_cache[i];
        }

        /* Compute a(i) for new indices */
        for (idx_t i = cache_size + 1; i <= new_size; ++i) {
            idx_t best_k = 1;
            int best_steps = -1;

            /* Compute Euclidean steps for all k = 1..i */
            for (idx_t k = 1; k <= i; ++k) {
                idx_t a0 = i;
                idx_t b0 = k;
                int steps = 0;
                /* Euclidean algorithm counting steps */
                while (b0 != 0) {
                    ++steps;
                    idx_t q = a0 / b0;
                    idx_t r = a0 - q * b0;   /* a0 % b0 */
                    a0 = b0;
                    b0 = r;
                }

                if (steps > best_steps) {
                    best_steps = steps;
                    best_k = k;
                }
                /* if steps equal best_steps we keep the smaller k
                   (current k is larger as loop increases) */
            }
            new_cache[i] = best_k;
        }

        /* Replace old cache */
        free(a_cache);
        a_cache = new_cache;
        cache_size = new_size;
    }

    /* Return the cached value for n */
    return a_cache[n];
}
