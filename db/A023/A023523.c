/*
 * A023523  a(n) = prime(n)*prime(n-1) + 1.
 * https://oeis.org/A023523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)*prime(n-1) + 1 (prime(0) = 1)
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n) {
    /* Handle the special case n == 1.
       prime(1) = 2, prime(0) = 1, so a(1) = 2*1 + 1 = 3. */
    if (n <= 1) {
        return (seq_t)2 * (seq_t)1 + (seq_t)1; /* 3 */
    }

    /* Cached primes to avoid re-sieving on multiple calls. */
    static uint32_t *primes = NULL;
    static idx_t cached_n = 0;

    /* If we have not yet computed enough primes, compute them. */
    if (n > cached_n) {
        /* Free any old array to avoid leaks. */
        if (primes != NULL) {
            free(primes);
            primes = NULL;
        }
        /* The utility function allocates the array and sets the count. */
        sieve_first_n_primes(n, &primes, &cached_n);
    }

    /* Compute a(n) = prime(n) * prime(n-1) + 1.
       primes array is 0-based: primes[0] == prime(1). */
    seq_t p_n     = (seq_t)primes[n - 1];   /* prime(n)   */
    seq_t p_n_1   = (seq_t)primes[n - 2];   /* prime(n-1) */
    return p_n * p_n_1 + (seq_t)1;
}
