/*
 * A231531  Imaginary part of Product_{k = 1..n} (k + i), i = sqrt(-1).
 * https://oeis.org/A231531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A231531:  Im(∏_{k=1..n} (k + i))
    a(0) = 0,  a(1) = 1
    Recurrence:
        a(n) = (2*n - 1)*a(n-1) - (n^2 - 2*n + 2)*a(n-2)
*/

#include <stddef.h>
#include <stdlib.h>

/* The caller supplies the integer types seq_t (value) and idx_t (index). */
seq_t a(idx_t n)
{
    /* Static cache for previously computed terms. */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;   /* allocated size of the cache */

    /* Base cases */
    if (n == 0u)
        return (seq_t)0;
    if (n == 1u)
        return (seq_t)1;

    /* Allocate or grow the cache to hold indices up to n */
    if (cache_len < n + 1u) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1u) * sizeof(seq_t));
        if (!tmp)
            /* Allocation failed – return 0 (unlikely for n ≤ 10000). */
            return (seq_t)0;
        cache      = tmp;
        cache_len  = n + 1u;
        /* Initialise the first two known values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* Compute terms from k = 2 up to n using the recurrence */
    for (idx_t k = 2u; k <= n; ++k) {
        seq_t seq_k = (seq_t)k;

        /* term1 = (2*k - 1) * a(k-1) */
        seq_t term1 = ( ( (seq_t)2 * seq_k ) - (seq_t)1 ) * cache[k - 1];

        /* term2 = (k^2 - 2*k + 2) * a(k-2) */
        seq_t k_sq        = seq_k * seq_k;
        seq_t term2_mul   = k_sq - ( (seq_t)2 * seq_k ) + (seq_t)2;
        seq_t term2       = term2_mul * cache[k - 2];

        cache[k] = term1 - term2;
    }

    return cache[n];
}
