/*
 * A162881  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 41*a(n-1) + 41*a(n-2) - 861*a(n-3) with initial values
 *            a(0)=1, a(1)=43, a(2)=1806, a(3)=74949
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised cache: cache[i] holds a(i).
       cache[0..next_to_compute-1] are computed.  */
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;      /* allocated size in elements   */
    static idx_t   next_to_compute = 0; /* first index not yet computed */

    /* Initialise the cache once with the first four terms. */
    if (!cache) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* OOM – never expected */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)43;
        cache[2] = (seq_t)1806;
        cache[3] = (seq_t)74949;
        next_to_compute = 4;                   /* next term to compute */
    }

    /* Grow the cache if the requested n exceeds current capacity. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;       /* OOM – never expected */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* If the value is already cached, return it immediately. */
    if (n < next_to_compute)
        return cache[n];

    /* Compute all missing terms up to n. */
    for (idx_t i = next_to_compute; i <= n; ++i) {
        /* Use seq_t arithmetic; intermediate values are handled
           correctly by the integer type chosen for seq_t.            */
        seq_t term = (seq_t)41 * cache[i - 1];
        term       += (seq_t)41 * cache[i - 2];
        term       -= (seq_t)861 * cache[i - 3];
        cache[i] = term;
    }

    next_to_compute = n + 1;
    return cache[n];
}
