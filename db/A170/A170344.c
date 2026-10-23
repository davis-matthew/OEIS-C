/*
 * A170344  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 45·a(n-1) + 45·a(n-2) + … + 45·a(n-41) - 1035·a(n-42) +
 *            N(n) where N(0)=1, N(1…41)=2, N(42)=1, N(n)=0 for n>42
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include allocation helpers provided by the build system */
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* sequence function */
seq_t a(idx_t n) {
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;

    /* initialise cache with a(0)=1 on first call */
    if (cache == NULL) {
        cache = seq_alloc_array(1);   /* size 1 -> cache[0] */
        cache[0] = (seq_t)1;
        max_computed = 0;
    }

    /* if needed extend cache up to index n */
    if (n > max_computed) {
        /* allocate a larger array */
        idx_t new_size = n + 1;
        seq_t *new_cache = seq_alloc_array(new_size);

        /* copy existing values */
        for (idx_t i = 0; i <= max_computed; ++i)
            new_cache[i] = cache[i];

        /* free old array */
        seq_free_array(cache);
        cache = new_cache;

        /* compute new terms */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t inhom = 0;
            if (i <= 41)
                inhom = (seq_t)2;
            else if (i == 42)
                inhom = (seq_t)1;
            /* initialise the sum with the inhomogeneous term */
            seq_t sum = inhom;

            /* add 45·a(i-k) for k = 1 … min(i,41) */
            idx_t max_k = (i < 41) ? i : 41;
            for (idx_t k = 1; k <= max_k; ++k) {
                sum += (seq_t)45 * cache[i - k];
            }

            /* subtract 1035·a(i-42) if i ≥ 42 */
            if (i >= 42) {
                seq_t term = (seq_t)1035 * cache[i - 42];
                sum -= term;
            }

            cache[i] = sum;
        }

        max_computed = n;
    }

    return cache[n];
}
