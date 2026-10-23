/*
 * A153422  Primes of the form k^2 + 15*k + 13.
 * https://oeis.org/A153422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of k^2 + 15k + 13 for k = 0,1,2,…
 * Range:     u8 n <= 10, u16 n <= 110, u32 n <= 113, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If n < 1 behavior is undefined but the function returns 0. */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Static cache of primes found so far. */
    static seq_t *cache   = NULL;   /* array of primes          */
    static size_t  cap    = 0;      /* allocated capacity       */
    static size_t  cnt    = 0;      /* number of primes stored  */
    static idx_t   next_k = 0;      /* next k to test           */

    /* Initialise cache on first call. */
    if (!cache) {
        cap   = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cnt   = 0;
        next_k = 0;
    }

    /* Generate primes until we have at least n of them. */
    while (cnt < n) {
        /* Compute f(k) = k^2 + 15k + 13 using seq_t arithmetic. */
        seq_t val = (seq_t)next_k * next_k + (seq_t)15 * next_k + (seq_t)13;

        if (is_prime_big(val)) {
            if (cnt == cap) {
                /* Grow the cache. */
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[cnt++] = val;
        }
        ++next_k;
    }

    return cache[n - 1];
}
