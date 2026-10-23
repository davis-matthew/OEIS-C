/*
 * A029938  a(n) = (p-5)(p-7)/24, where p=prime(n).
 * https://oeis.org/A029938
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, nice, changed
 * Formula:   a(n) = (prime(n)-5)*(prime(n)-7)/24 where prime(n) is the n‑th
 *            prime
 * Range:     u8 n <= 8, u16 n <= 130, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sieve_first_n_primes.h"
#include <stdlib.h>

static uint32_t *prime_cache      = NULL;   /* cached primes (zero‑based) */
static size_t   prime_cache_words = 0;      /* number of cached primes */

/*  Return the n‑th term of OEIS A029938.  */
seq_t a(idx_t n)
{
    /* For indices below the OEIS offset the value is undefined; return 0. */
    if (n < 3)
        return 0;

    /* Rebuild the cache if it does not yet contain at least n primes. */
    if (prime_cache_words < (size_t)n) {
        uint32_t *tmp = NULL;
        size_t    cnt = 0;
        sieve_first_n_primes(n, &tmp, &cnt);   /* provides the first n primes */
        free(prime_cache);                     /* discard the old cache */
        prime_cache      = tmp;
        prime_cache_words = cnt;
    }

    /* Retrieve the n‑th prime; the cache is zero‑based. */
    uint32_t p = prime_cache[n - 1];

    /* Compute (p‑5)(p‑7)/24 using seq_t arithmetic. */
    seq_t p_seq = (seq_t)p;
    seq_t val   = (p_seq - (seq_t)5) * (p_seq - (seq_t)7);
    val /= (seq_t)24;

    return val;
}
