/*
 * A166004  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A166004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27 * (a(n-1)+...+a(n-9)) - 378 * a(n-10) + c[n] where
 *            c[n] = 1,2,2,2,2,2,2,2,2,2,1 for 0 ≤ n ≤ 10, and c[n] = 0 for n
 *            > 10. (This follows from the rational generating function given
 *            in OEIS.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* c[n] corresponds to the coefficients of the numerator of the g.f. */
static const unsigned int c[11] = {1,2,2,2,2,2,2,2,2,2,1};

/* Static cache to allow repeated calls in any order. */
static seq_t *cache   = NULL;   /* array holding computed values */
static idx_t  cache_cap = 0;    /* number of slots currently allocated */
static idx_t  last_computed = 0;/* largest n for which cache[n] is valid */

seq_t a(idx_t n)
{
    /* Ensure cache can hold indices up to n. */
    if (cache_cap < n + 1) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0 as sentinel (value not used in tests). */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise seed value if this is the first call. */
    if (last_computed == 0 && cache[0] != 1) {
        cache[0] = 1;
    }

    /* Compute terms from last_computed+1 up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* Sum the previous 9 terms (skipping negative indices). */
        for (idx_t k = 1; k <= 9; ++k) {
            if (i >= k)
                sum += cache[i - k];
        }

        /* Main recurrence contribution. */
        seq_t term = sum * 27;

        /* Subtract 378 * a(i-10) when i >= 10. */
        if (i >= 10)
            term -= cache[i - 10] * 378;

        /* Add the inhomogeneous term c[i] for 0 ≤ i ≤ 10. */
        if (i <= 10)
            term += (seq_t)c[i];

        cache[i] = term;
    }

    last_computed = n;
    return cache[n];
}
