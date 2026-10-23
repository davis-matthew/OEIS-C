/*
 * A023515  a(n) = prime(n)*prime(n-1) - 1.
 * https://oeis.org/A023515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * prime(n-1) - 1 (for n ≥ 1, prime(0) is taken
 *            to be 1)
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/sieve_first_n_primes.h"

/* Cached prime array (prime_cache[0] = prime(1) = 2). */
static uint32_t *prime_cache = NULL;   /* array allocated by sieve_first_n_primes */
static size_t   prime_cache_cnt = 0;   /* number of primes currently cached */

/* Ensure that we have at least n primes stored in prime_cache. */
static void ensure_primes(idx_t n)
{
    /* No need to grow if we already have enough. */
    if ((size_t)n <= prime_cache_cnt)
        return;

    /* Free existing cache before creating a new one. */
    if (prime_cache != NULL)
        free(prime_cache);

    /* Request exactly n primes from the helper. */
    sieve_first_n_primes(n, &prime_cache, &prime_cache_cnt);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* According to OEIS, the sequence is defined for n ≥ 1.  For n == 0 we
       return 0 as a harmless default. */
    if (n == 0)
        return 0;

    ensure_primes(n);

    seq_t p_n   = (seq_t)prime_cache[(size_t)n - 1];          /* prime(n)   */
    seq_t p_nm1 = (n == 1) ? (seq_t)1                        /* prime(0) = 1 */
                          : (seq_t)prime_cache[(size_t)n - 2];/* prime(n‑1) */

    return p_n * p_nm1 - 1;
}
