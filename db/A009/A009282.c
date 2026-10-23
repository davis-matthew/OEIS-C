/*
 * A009282  Expansion of e.g.f.: exp(x + sin(x)).
 * https://oeis.org/A009282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + Σ_{k=0..⌊(n-1)/2⌋} (-1)^k * binomial(n-1,2k) *
 *            a(n-2k-1) with a(0) = 1. This implementation memoises the values
 *            so any call order is supported
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 17, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Computes the n‑th term of the sequence.  */
seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0)
        return (seq_t)1;

    /* Static cache for memoisation */
    static seq_t *cache = NULL; /* cache[i] = a(i) for 0 <= i <= cache_cap-1 */
    static size_t cache_cap = 0;    /* number of cached entries */
    static idx_t max_index = 0; /* largest n that has been computed */

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        assert(tmp);                /* abort on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise a(0) once cache is ready */
    if (cache_cap > 0 && cache[0] == 0)
        cache[0] = (seq_t)1;

    /* Compute missing values up to n */
    for (idx_t m = max_index + 1; m <= n; ++m) {
        seq_t sum = 0;

        idx_t limit = (m - 1) >> 1;   /* floor((m-1)/2) */
        for (idx_t k = 0; k <= limit; ++k) {
            idx_t idx2k = 2 * k;
            seq_t coeff = binomial(m - 1, idx2k);
            seq_t term  = coeff * cache[m - 1 - idx2k];
            if (k & 1)
                sum -= term;
            else
                sum += term;
        }

        cache[m] = cache[m - 1] + sum;
    }

    max_index = n;
    return cache[n];
}
