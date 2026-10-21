/*
 * A203583  Vandermonde sequence using x^2 - xy + y^2 applied to
 *          (2,3,...,n+1).
 * https://oeis.org/A203583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{2 ≤ i < j ≤ n+1} (i^2 - i·j + j^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Assumptions: 
   - seq_t and idx_t are defined externally by the build.
   - seq_t supports +, -, *, /, %, comparisons, and standard integer
     literals.
   - idx_t is at least 64‑bit (unsigned). */

seq_t a(idx_t n)
{
    /* Static cache of already computed terms. */
    static seq_t *cache    = NULL; /* 1‑based array: cache[1] = a(1) */
    static size_t cache_cap = 0;   /* allocated array size (indices 0..cap-1) */
    static size_t last_computed = 0; /* highest n that has been calculated */

    if (n < 1) /* undefined, but return 0 for safety */
        return (seq_t)0;

    /* Ensure storage is large enough (indices up to n) */
    if (cache == NULL || cache_cap <= (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1) + 4; /* a small safety margin */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first term if this is the very first call */
    if (last_computed == 0) {
        cache[1] = (seq_t)1;          /* a(1) = 1 */
        last_computed = 1;
    }

    /* Compute any newly required terms incrementally */
    if ((size_t)n > last_computed) {
        for (size_t k = last_computed + 1; k <= (size_t)n; ++k) {
            seq_t prod = (seq_t)1;                /* product over new pairs */
            seq_t j_val = (seq_t)(k + 1);         /* the new element j = n+1 */

            /* Multiply by ∏_{i=2}^{k} (i^2 - i*j + j^2) */
            for (idx_t i = 2; i <= (idx_t)k; ++i) {
                seq_t i_sq = (seq_t)i * (seq_t)i;
                seq_t j_sq = j_val * j_val;
                seq_t ij   = (seq_t)i * j_val;
                /* i^2 + j^2 - i*j is always positive for i<j */
                seq_t term = i_sq + j_sq - ij;
                prod *= term;
            }
            cache[k] = cache[k - 1] * prod; /* a(k) = a(k-1) * new_product */
        }
        last_computed = (size_t)n; /* update highest computed index */
    }

    return cache[n];
}
