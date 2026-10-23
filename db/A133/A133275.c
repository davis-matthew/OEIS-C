/*
 * A133275  Numbers X such that 30*X^2-45 is a square.
 * https://oeis.org/A133275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n+1) - a(n+2) Initial values: a(1) = 3, a(2) = 63
 *            (Recurrence: a(n+2) = 22*a(n+1) - a(n) for n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[1] … cache[n] store the sequence terms (cache[0] unused) */
    static seq_t *cache = NULL;
    static idx_t  max_computed = 0;   /* largest index that has been computed */
    static size_t cache_cap   = 0;     /* allocated capacity of cache */

    if (n == 0) return 0; /* out of bounds, not used in tests */

    /* Allocate or enlarge cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 2 : cache_cap * 2;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();            /* memory allocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize first terms if needed */
    if (max_computed == 0) {
        cache[1] = (seq_t)3;
        max_computed = 1;
    }
    if (max_computed < 2 && n >= 2) {
        cache[2] = (seq_t)63;
        max_computed = 2;
    }

    /* Compute terms up to requested n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* a(i) = 22*a(i-1) - a(i-2)            */
        seq_t prod = cache[i-1] * (seq_t)22;
        cache[i]   = prod - cache[i-2];
    }
    max_computed = n;

    return cache[n];
}
