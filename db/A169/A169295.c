/*
 * A169295  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0) = 1  
   for 1 ≤ n ≤ 28:   a(n) = 2 + 40 * Σ_{k=0}^{n-1} a(k)  
   a(29) = 1 + 40 * Σ_{k=1}^{28} a(k) – 820 * a(0)  
   for n ≥ 30:        a(n) = 40 * Σ_{k=1}^{28} a(n-k) – 820 * a(n-29)  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache that grows as needed. */
    static seq_t *cache = NULL;
    static idx_t  max_computed = (idx_t)-1;
    static size_t cache_cap   = 0;

    /* Ensure there is an initial entry a(0). */
    if (cache == NULL) {
        cache_cap = 64;                     /* initial arbitrary size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);                /* allocation failure → terminate */
        cache[0] = 1;                       /* a(0) = 1 */
        max_computed = 0;
    }

    /* Expand cache if n is beyond the current allocation. */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache   = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed. */
    if (n <= max_computed) return cache[(size_t)n];

    /* Compute all missing values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        size_t ii = (size_t)i;      /* cast for array index */
        seq_t sum = 0;

        if (i <= 28) {
            /* a(i) = 2 + 40 * Σ_{k=1}^{i} a(i-k)   (indices i-1 … 0) */
            for (idx_t k = 1; k <= i; ++k)
                sum += cache[ii - (size_t)k];
            cache[ii] = 2 + 40 * sum;
        } else if (i == 29) {
            /* a(29) = 1 + 40 * Σ_{k=1}^{28} a(29-k) – 820 * a(0) */
            for (idx_t k = 1; k <= 28; ++k)
                sum += cache[ii - (size_t)k];
            cache[ii] = 1 + 40 * sum - 820 * cache[0];
        } else { /* i >= 30 */
            /* a(i) = 40 * Σ_{k=1}^{28} a(i-k) – 820 * a(i-29) */
            for (idx_t k = 1; k <= 28; ++k)
                sum += cache[ii - (size_t)k];
            cache[ii] = 40 * sum - 820 * cache[ii - 29];
        }
    }

    max_computed = n;
    return cache[(size_t)n];
}
