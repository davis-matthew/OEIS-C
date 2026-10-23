/*
 * A015203  Sum of Gaussian binomial coefficients for q=15.
 * https://oeis.org/A015203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, a(n)=2*a(n-1)+a(n-2)*((15^(n-1))-1) for n>=2
 *            (OEIS A015203)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Static cache */
    static seq_t *cache      = NULL;
    static idx_t  cache_size = 0;
    static seq_t pow15       = 0;   /* 15^(current_index-1) */

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache      = (seq_t *)malloc(2 * sizeof(seq_t));
        cache[0]   = (seq_t)1;
        cache[1]   = (seq_t)2;
        cache_size = 2;
        pow15      = (seq_t)15;      /* 15^(1) for computing a(2) */
    }

    /* Grow cache if needed */
    if (n >= cache_size) {
        idx_t i;
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;  /* Allocation failure */
        cache = new_cache;

        /* Compute missing values */
        for (i = cache_size; i <= n; ++i) {
            seq_t term_prev_prev = cache[i - 2];
            seq_t term_prev      = cache[i - 1];
            /* a(n) = 2*a(n-1) + a(n-2)*((15^(n-1))-1) */
            seq_t contrib = term_prev_prev * (pow15 - (seq_t)1);
            seq_t val     = term_prev * (seq_t)2 + contrib;
            cache[i] = val;
            /* Update 15^(i-1) for next iteration */
            pow15 = pow15 * (seq_t)15;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
