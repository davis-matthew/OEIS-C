/*
 * A133061  a(n) = 5*p^5 - 3*p^3 - 2*p^2, where p = prime(n).
 * https://oeis.org/A133061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*p^5 - 3*p^3 - 2*p^2 where p = A000040(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 18, u64 n <= 688, u128 n <= 1000,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* Cache of first primes for fast access. */
    static uint32_t *prime_cache = NULL;
    static size_t prime_cache_size = 0;

    if (n == 0) return 0; /* offset is 1; n>=1 for valid calls */

    /* Ensure the cache contains at least the first n primes. */
    if ((size_t)n > prime_cache_size) {
        free(prime_cache);            /* discard old cache if any */
        sieve_first_n_primes(n, &prime_cache, &prime_cache_size);
        /* After this call, prime_cache_size == n (sieve_first_n_primes does so). */
    }

    /* Retrieve the n-th prime (prime_cache is 0‑indexed). */
    uint32_t p_u32 = prime_cache[n - 1];
    seq_t p   = (seq_t)p_u32;   /* cast to seq_t early to avoid overflow in strings */
    seq_t p2  = p * p;          /* p^2 */
    seq_t p3  = p2 * p;         /* p^3 */
    seq_t p5  = p3 * p2;        /* p^5 = p^3 * p^2 */

    /* Compute the formula. */
    seq_t result = (seq_t)5 * p5 - (seq_t)3 * p3 - (seq_t)2 * p2;
    return result;
}
