/*
 * A179078  a(n) = ((3^p - 3)/p) mod p where p is n-th prime.
 * https://oeis.org/A179078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((3^p - 3)/p) mod p where p is the nth prime
 * Range:     u8 n <= 66, u16 n <= 6762, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   powmod64, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/powmod64.h"
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* Periodically cache the list of primes up to the largest n seen so far */
    static uint32_t *primes = NULL;
    static size_t prime_count = 0;

    /* Ensure we have at least n primes cached */
    if ((size_t)n > prime_count) {
        size_t new_count = 0;
        if (primes) free(primes);               /* free old cache */
        sieve_first_n_primes(n, &primes, &new_count);
        prime_count = new_count;
    }

    /* Retrieve the nth prime (1‑based index) */
    uint64_t p = (uint64_t)primes[n - 1];

    /* Compute 3^p modulo p^2 */
    uint64_t mod = p * p;                      /* p^2 fits in 64 bits */
    uint64_t pow_mod = powmod64(3, p, mod);

    /* Compute (3^p - 3) modulo p^2; the result is divisible by p */
    uint64_t diff = (pow_mod >= 3) ? (pow_mod - 3) : (pow_mod + mod - 3);

    /* The desired value is (3^p - 3)/p modulo p, which lies in [0, p-1] */
    seq_t result = (seq_t)(diff / p);
    return result;
}
