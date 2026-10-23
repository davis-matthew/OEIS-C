/*
 * A138407  a(n) = p^4*(p-1), where p = prime(n).
 * https://oeis.org/A138407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^4 * (prime(n) - 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *prime_cache = NULL;   /* first n primes */
static idx_t prime_cache_size = 0;     /* number of primes cached */

/* Return the n-th term of OEIS A138407 */
seq_t a(idx_t n)
{
    /* Invalid indices below offset: return 0 */
    if (n < 1) return (seq_t)0;

    /* Ensure the cache contains at least n primes */
    if (n > prime_cache_size) {
        uint32_t *new_cache = NULL;
        size_t count = 0;
        sieve_first_n_primes(n, &new_cache, &count);

        if (prime_cache) free(prime_cache);
        prime_cache = new_cache;
        prime_cache_size = (idx_t)count;
    }

    /* Retrieve the n-th prime (offset 1) */
    seq_t p = (seq_t)prime_cache[n - 1];

    /* Compute p^4 * (p - 1) */
    seq_t p2 = p * p;           /* p^2 */
    seq_t p4 = p2 * p2;         /* p^4 */
    seq_t result = p4 * (p - 1);

    return result;
}
