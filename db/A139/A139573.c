/*
 * A139573  Numbers with 41 divisors.
 * https://oeis.org/A139573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^40 (n ≥ 1)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 4, big any n
 * Helpers:   big_pow, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes.h"
#include "../utils/big_pow.h"

static idx_t *prime_cache = NULL;   /* array of primes */
static size_t prime_cache_cnt = 0;  /* number of primes stored */

/* Ensure that at least n primes are available in prime_cache. */
static void
ensure_prime_cache(idx_t n)
{
    if (prime_cache_cnt >= n)
        return;                  /* already have enough */

    idx_t limit = (n < 6) ? 15 : n * 20;  /* rough upper bound for the n‑th prime */

    while (1) {
        free(prime_cache);        /* discard previous cache if any */
        prime_cache = NULL;
        prime_cache_cnt = 0;

        build_primes(limit, &prime_cache, &prime_cache_cnt);

        if (prime_cache_cnt >= n)
            return;               /* sufficient primes obtained */

        limit *= 2;              /* increase the bound and try again */
    }
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* out of bounds – return 0 */

    ensure_prime_cache(n);

    idx_t p = prime_cache[n - 1];          /* nth prime (1‑based indexing) */
    return big_pow((seq_t)p, (seq_t)40);   /* compute p^40 */
}
