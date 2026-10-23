/*
 * A131993  a(n) = 1 + prime(n) + prime(n)^2 + prime(n)^3 + prime(n)^4 +
 *          prime(n)^5.
 * https://oeis.org/A131993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + prime(n) + prime(n)^2 + prime(n)^3 + prime(n)^4 +
 *            prime(n)^5
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 23, u64 n <= 914, u128 n <= 1000,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation caches the first N primes and rebuilds the cache
   only when a larger prime index is requested. */

#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *prime_cache = NULL;
static size_t     prime_cache_cnt = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* sequence defined for n ≥ 1 */

    /* Grow the cache if necessary. */
    if ((size_t)n > prime_cache_cnt) {
        uint32_t *new_p = NULL;
        size_t   new_cnt = 0;
        sieve_first_n_primes(n, &new_p, &new_cnt);

        /* Invariant: new_cnt == n.  If not, emit 0 and keep cache empty. */
        if (new_cnt < (size_t)n) {
            if (prime_cache) free(prime_cache);
            prime_cache     = NULL;
            prime_cache_cnt = 0;
            return 0;
        }

        if (prime_cache) free(prime_cache);
        prime_cache     = new_p;
        prime_cache_cnt = new_cnt;
    }

    /* Retrieve the nth prime (1‑based index). */
    seq_t p  = (seq_t)prime_cache[n - 1];
    seq_t p2 = p * p;
    seq_t p3 = p2 * p;
    seq_t p4 = p3 * p;
    seq_t p5 = p4 * p;

    return 1 + p + p2 + p3 + p4 + p5;
}
