/*
 * A039703  a(n) = n-th prime modulo 5.
 * https://oeis.org/A039703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime modulo 5 (OEIS A039703)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a sieve to generate the first n primes on demand. */

#include <stdlib.h>           /* free */
#include "../utils/sieve_first_n_primes.h"   /* sieve_first_n_primes */

seq_t a(idx_t n)
{
    /* Offset starts at 1; behaviour for n==0 is irrelevant. */
    if (n == 0) return 0;

    /* Static cache of the primes generated so far. */
    static uint32_t *prime_cache = NULL;
    static size_t prime_cache_cnt = 0;

    /* If we do not yet have n primes, generate more. */
    if (prime_cache_cnt < (size_t)n) {
        uint32_t *new_primes = NULL;
        size_t new_cnt = 0;

        sieve_first_n_primes(n, &new_primes, &new_cnt);

        /* Replace the old cache. */
        if (prime_cache) free(prime_cache);
        prime_cache  = new_primes;
        prime_cache_cnt = new_cnt;   /* should equal n */
    }

    /* Return the (n-1)-th stored prime modulo 5. */
    return (seq_t)(prime_cache[n - 1] % 5);
}
