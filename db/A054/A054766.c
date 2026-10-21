/*
 * A054766  a(n+2) = (2*n + 3)*a(n+1) + (n + 1)^2*a(n), a(0) = 1, a(1) = 0.
 * https://oeis.org/A054766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=0; a(n+2) = (2*n+3)*a(n+1) + (n+1)^2*a(n) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored (cache_size = max_index+1) */

    /* initialise cache with the two seed values if needed */
    if (cache_size == 0) {
        cache_size = 2;                     /* we will store a[0] and a[1] */
        cache = (seq_t*)calloc(cache_size, sizeof(seq_t));
        if (!cache) abort();                /* out of memory */
        cache[0] = (seq_t)1;                /* a(0) */
        cache[1] = (seq_t)0;                /* a(1) */
    }

    /* if n is already cached, simply return it */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* extend cache to hold values up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    /* compute values from current cache_size up to n */
    for (size_t k = cache_size; k <= (size_t)n; ++k) {
        if (k == 0) {
            cache[k] = (seq_t)1;   /* already set during init, but kept for completeness */
            continue;
        }
        if (k == 1) {
            cache[k] = (seq_t)0;
            continue;
        }

        /* recurrence: a(k) = (2k-1)*a(k-1) + (k-1)^2*a(k-2) */
        seq_t coeff1 = (seq_t)(2 * k - 1);   /* (2k-1) multiplies a(k-1) */
        seq_t coeff2 = (seq_t)(k - 1);       /* (k-1) squared multiplies a(k-2) */
        coeff2 = coeff2 * coeff2;

        cache[k] = coeff1 * cache[k - 1] + coeff2 * cache[k - 2];
    }

    cache_size = new_size;
    return cache[(size_t)n];
}
