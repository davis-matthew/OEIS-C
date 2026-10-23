/*
 * A196202  a(n) = 2^(prime(n)-1) mod prime(n)^2.
 * https://oeis.org/A196202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(prime(n)-1) mod prime(n)^2 where prime(n) is the n-th
 *            prime (prime(1)=2)
 * Range:     u8 n <= 6, u16 n <= 55, u32 n <= 6593, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {           /* offset is 1, but guard against zero */
        return 0;
    }

    /* static storage of primes up to the maximum n requested so far */
    static uint32_t *primes = NULL;    /* array of the first primes */
    static size_t  primes_cnt  = 0;    /* number of primes currently in array */

    /* If we have not yet computed enough primes, recompute */
    if ((size_t)n > primes_cnt) {
        free(primes);                       /* free any previous allocation */
        sieve_first_n_primes((idx_t)n, &primes, &primes_cnt);
    }

    /* Retrieve the n-th prime; indices are 0‑based in the array */
    uint64_t p = (uint64_t)primes[n - 1];

    /* Compute modulus p^2 */
    seq_t mod = (seq_t)p * (seq_t)p;

    /* Compute 2^(p-1) mod p^2 */
    return mod_pow((seq_t)2, p - 1, mod);
}
