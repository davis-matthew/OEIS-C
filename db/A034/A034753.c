/*
 * A034753  Dirichlet convolution of 3^(n-1) with sigma(n).
 * https://oeis.org/A034753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n} σ(n/d) · 3^(d-1) (n ≥ 1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Cached data */
static seq_t *sigma_arr   = NULL;   /* sigma_arr[i] = σ(i)          */
static seq_t *pow3_arr    = NULL;   /* pow3_arr[e]  = 3^e           */
static seq_t *a_cache     = NULL;   /* a_cache[n]   = a(n)           */
static idx_t  max_loaded  = 0;      /* highest index for which arrays are valid */

/* Forward declaration */
static void ensure_precomputed(idx_t limit);

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined element, return 0 */
        return (seq_t)0;

    if (n > max_loaded)             /* compute values up to n */
        ensure_precomputed(n);

    return a_cache[n];
}

/* Ensure that sigma_arr, pow3_arr and a_cache contain values up to 'limit' */
static void ensure_precomputed(idx_t limit)
{
    /* Allocate new larger arrays.  Cast of malloc is allowed for C/C++ duality. */
    seq_t *new_sigma = (seq_t*)calloc((size_t)(limit + 1), sizeof(seq_t));
    seq_t *new_pow3  = (seq_t*)calloc((size_t)(limit + 1), sizeof(seq_t));
    seq_t *new_cache = (seq_t*)calloc((size_t)(limit + 1), sizeof(seq_t));

    /* Populate σ(i) for i = 1 … limit */
    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i)
            new_sigma[j] += (seq_t)i;
    }

    /* Compute 3^e for e = 0 … limit */
    new_pow3[0] = (seq_t)1;
    for (idx_t e = 1; e <= limit; ++e)
        new_pow3[e] = new_pow3[e - 1] * (seq_t)3;

    /* Compute a(n) for n = 1 … limit */
    for (idx_t m = 1; m <= limit; ++m) {
        seq_t sum = (seq_t)0;
        /* iterate over all divisors d of m */
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                idx_t d1 = d;
                idx_t d2 = m / d;

                /* contribution from divisor d1 */
                sum += new_sigma[d2] * new_pow3[d1 - 1];

                /* if the divisor pair d1, d2 are distinct, add the symmetrical term */
                if (d1 != d2)
                    sum += new_sigma[d1] * new_pow3[d2 - 1];
            }
        }
        new_cache[m] = sum;
    }

    /* Replace old arrays (freeing memory) */
    free(sigma_arr);
    free(pow3_arr);
    free(a_cache);

    sigma_arr = new_sigma;
    pow3_arr  = new_pow3;
    a_cache   = new_cache;
    max_loaded = limit;
}
