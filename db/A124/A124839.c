/*
 * A124839  Inverse binomial transform of the Moebius sequence {mu(k), k >=
 *          1}, A008683.
 * https://oeis.org/A124839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n-1} (-1)^(n-1-k) * binomial(n-1,k) *
 *            mobius(k+1)
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 31, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   binomial, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/mobius.h"

/* External linkage function defined by the build system */
seq_t a(idx_t n)
{
    /* a(1) is the first term; return 0 for indices below the offset. */
    if (n == 0)
        return 0;

    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* Number of terms stored (1‑based index). */
    static idx_t cache_cap = 0;   /* Allocated capacity. */

    /* Extend the cache if necessary. */
    if (n > cache_cap)
    {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort();                       /* Allocation failed. */
        cache = new_cache;
        cache_cap = (idx_t)new_cap;
    }

    /* Compute values up to n if not already done. */
    while (cache_len < n)
    {
        idx_t i = cache_len + 1;          /* Current term number (1‑based). */
        seq_t sum = 0;
        idx_t n_minus_1 = i - 1;          /* n-1 for this term. */

        for (idx_t k = 0; k < i; ++k)
        {
            seq_t mu_val = mobius(k + 1);      /* mu(k+1) */
            if (mu_val == 0)
                continue;

            seq_t binom = binomial(n_minus_1, k);
            seq_t term = mu_val * binom;

            if ((n_minus_1 - k) & 1)
                sum -= term;
            else
                sum += term;
        }

        cache[cache_len] = sum;
        ++cache_len;
    }

    return cache[n - 1];
}
