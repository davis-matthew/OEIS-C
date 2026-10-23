/*
 * A169500  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169500: a(n) = number of reduced words of length n in the Coxeter
 * group on 7 generators with relations (S_i)^2 = (S_i S_j)^34 = I.
 * The generating function is
 *    (1 + 2t + 2t^2 + … + 2t^33 + t^34) / (1 - 5t - 5t^2 - … - 5t^33 + 15t^34).
 * From (1 - 5t - … - 5t^33 + 15t^34) * G(t) = 1 + 2t + … + 2t^33 + t^34
 * we obtain the linear recurrence
 *     a(n) = 5∑_{i=1}^{33} a(n-i)  - 15 a(n-34) + p(n)               (n≥0)
 * where the “inhomogeneous” term p(n) equals
 *     1  if n = 0 or n = 34
 *     2  if 1 ≤ n ≤ 33
 *     0  otherwise.
 *
 * The implementation memoizes values in a dynamically growing array, storing
 * all terms up to the largest n requested.  It works for any integer type
 * seq_t and idx_t provided by the build environment, including arbitrary-precision
 * integer classes.  No operations other than integer arithmetic are used.
 */

#include <stddef.h>
#include <stdlib.h>

/* External types supplied by the build system.  Do not define them here. */
seq_t a(idx_t n);

/* Return the coefficient p(n) of the numerator generating function. */
static seq_t p_value(idx_t n)
{
    if (n == 0 || n == 34) return (seq_t)1;
    if (n <= 33)             return (seq_t)2;
    return (seq_t)0;
}

/* Cache of computed a(n) values.  Initialized lazily. */
static seq_t *cache      = NULL;     /* array of a(n) */
static size_t cache_size = 0;        /* current allocated size */
static idx_t  max_computed = (idx_t)-1; /* highest index stored */

seq_t a(idx_t n)
{
    /* Expand the cache if necessary. */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* Sum the previous 33 terms (or fewer if i<33). */
        idx_t kmax = (i >= 33) ? 33 : i;
        for (idx_t k = 1; k <= kmax; ++k)
            sum += cache[i - k];

        seq_t val = p_value(i);
        val = val + (seq_t)5 * sum;
        if (i >= 34)
            val = val - (seq_t)15 * cache[i - 34];

        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
