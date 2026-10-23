/*
 * A138402  a(n) = (n-th prime)^4-(n-th prime)^2.
 * https://oeis.org/A138402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p_n)^4 - (p_n)^2, where p_n is the n-th prime. Indexed
 *            from 1: a(1) = 12
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 54, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* Cache of primes.  The array is reallocated as needed. */
    static uint32_t *primes = NULL;
    static idx_t primes_nmax = 0;

    if (n == 0)
        return 0; /* undefined for n<1, but safe */

    /* Grow the cache if we need more primes. */
    if (n > primes_nmax) {
        if (primes)
            free(primes);              /* free previous buffer */
        uint32_t *tmp = NULL;
        size_t cnt = 0;
        sieve_first_n_primes(n, &tmp, &cnt);
        primes = tmp;
        primes_nmax = n;
    }

    /* Retrieve p_n (1‑based) from the cache (0‑based array). */
    uint32_t p = primes[n - 1];

    /* Compute p^4 - p^2 in seq_t, avoiding overflow in intermediate
       steps when seq_t is wide enough (auto‑promoted). */
    seq_t pp = (seq_t)p;
    seq_t p2 = pp * pp;
    seq_t p4 = p2 * p2;
    return p4 - p2;
}
