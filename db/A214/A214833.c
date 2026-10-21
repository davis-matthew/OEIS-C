/*
 * A214833  Number of formula representations of n using addition,
 *          multiplication and the constant 1.
 * https://oeis.org/A214833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n-1} a(i)*a(n-i) + Sum_{d|n, 1<d<n}
 *            a(d)*a(n/d); a(1)=1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
static seq_t *cache = NULL;      /* array of computed values, index 0 unused */
static size_t cap = 0;           /* current allocated capacity (>= n+1) */
static idx_t max_n = 0;          /* maximum n that has been computed so far */

/* External linkage required */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for n = 0, but return 0 safely */
        return (seq_t)0;

    /* Allocate or grow the cache to hold indices up to n */
    if (cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();   /* fatal if memory allocation fails */
        cache = new_cache;
        cap = new_cap;
    }

    /* Ensure a(1) is initialized */
    if (max_n < (idx_t)1) {
        cache[1] = (seq_t)1;
        max_n = 1;
    }

    /* If the requested term is already computed, return it */
    if (max_n >= n)
        return cache[n];

    /* Compute values for all indices from max_n+1 up to n */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t sum1 = 0;
        for (idx_t i = 1; i < k; ++i)
            sum1 += cache[i] * cache[k - i];

        seq_t sum2 = 0;
        for (idx_t d = 2; (d * d) <= k; ++d) {
            if (k % d == 0) {
                idx_t d2 = k / d;
                seq_t prod = cache[d] * cache[d2];
                sum2 += prod;          /* contribution for divisor d */
                if (d2 != d)           /* add the counterpart d2 separately */
                    sum2 += prod;
            }
        }

        cache[k] = sum1 + sum2;
    }

    max_n = n;
    return cache[n];
}
