/*
 * A052589  a(n) = (2^n - 1)*n!.
 * https://oeis.org/A052589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n - 1) * n! (offset 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms, factorial and 2^n values. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* allocated size of cache */
    static idx_t next_to_compute = 0; /* next index that has not been computed yet */
    static seq_t fact = 1;   /* factorial of (next_to_compute - 1) */
    static seq_t pow2 = 1;   /* 2^(next_to_compute - 1) */

    /* Extend cache if necessary. */
    if (cache_len <= (size_t)n)
    {
        size_t new_len = (n + 1) * 2;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }

    /* If the cache has not been initialised fully, initialise a(0). */
    if (next_to_compute == 0)
    {
        cache[0] = 0;            /* a(0) = (2^0 - 1) * 0! = 0 */
        next_to_compute = 1;     /* next term to compute is a(1) */
        /* fact and pow2 are already 1, corresponding to (next_to_compute-1)! and 2^(next_to_compute-1). */
    }

    /* Compute all terms up to n (if needed). */
    while (next_to_compute <= n)
    {
        idx_t k = next_to_compute;     /* current index */

        /* Update factorial and power of two for this index. */
        fact *= (seq_t)k;       /* fact becomes k! */
        pow2 *= (seq_t)2;       /* pow2 becomes 2^k */

        seq_t tmp = pow2 - (seq_t)1;    /* 2^k - 1 */
        cache[k] = tmp * fact;          /* a(k) = (2^k - 1) * k! */

        next_to_compute++;
    }

    return cache[n];
}
