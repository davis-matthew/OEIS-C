/*
 * A170378  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170378: Number of reduced words of length n in the Coxeter
   group with 33 generators and relations (S_i)^2 = (S_i S_j)^4 = I.
   The generating function is
        P(t)/Q(t)  with  P(t)=t^43+2t^42+…+2t+1
                       Q(t)=496t^43-31t^42-…-31t+1.
   The coefficients satisfy the linear recurrence
        a(0) = 1
        coeff_k = 1 (k=0), 2 (1≤k≤42), 1 (k=43), 0 otherwise
        a(k) = coeff_k + 31·∑_{i=1}^{min(42,k)} a(k-i)
                – (k≥43 ? 496·a(k-43) : 0).
   The function below memoises computed values and returns a(n) for any
   n≥0 in O(n) time. */

#include <stdlib.h>
#include <stddef.h>

/* Static cache of already computed terms.  */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of cached terms (0 means none) */

/* Ensure that cache contains terms up to index n.  */
static void
ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    /* Resize the cache. */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;

    /* Compute new terms. */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t coeff;
        if (k == 0)
            coeff = 1;
        else if (k <= 42)
            coeff = 2;
        else if (k == 43)
            coeff = 1;
        else
            coeff = 0;

        /* Sum of the previous up to 42 terms. */
        seq_t sum = 0;
        idx_t m = (k < 42) ? k : 42;
        for (idx_t j = 1; j <= m; ++j)
            sum += cache[k - j];

        seq_t val = coeff + (seq_t)31 * sum;
        if (k >= 43)
            val -= (seq_t)496 * cache[k - 43];

        cache[k] = val;
    }

    cache_len = new_len;
}

/* Public interface required by the build system. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
