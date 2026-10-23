/*
 * A141208  a(n) = prime(prime(prime(n) - 1) - 1) - 1, where prime(n) = n-th
 *          prime.
 * https://oeis.org/A141208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(prime(prime(n)-1)-1) - 1
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// Includes required by the utilities and the standard library
#include <stdlib.h>
#include <math.h>
#include "../utils/sieve_primes.h"

/* Cache of computed primes.
   prime_cache_cnt   : number of primes stored
   prime_cache_limit : sieve limit used to generate prime_cache
   prime_cache       : array of primes (1‑indexed by value)
*/
static idx_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static idx_t prime_cache_limit = 0;

// Forward declaration of a helper that returns the n‑th prime
static idx_t nth_prime(idx_t n);

/* Internal helper: extend the prime cache so that it contains at least
   'needed' primes.  It uses the common upper bound n(log n + log log n)
   for the n‑th prime (valid for n ≥ 6).  For smaller n a tiny limit is
   used.  The cache is regenerated with a larger sieve whenever the
   current one is insufficient. */
static void extend_prime_cache_to(size_t needed)
{
    if (prime_cache_cnt >= needed)
        return;

    /* Compute a suitable upper bound for the prime value */
    idx_t limit;
    if (needed < 6)
        limit = 15;
    else {
        double dn = (double)needed;
        double lim = dn * (log(dn) + log(log(dn))) + 3.0;
        limit = (idx_t)lim;
    }

    /* Keep the previous limit to avoid shrinking */
    if (limit <= prime_cache_limit)
        limit = prime_cache_limit * 2 + 10;

    /* Sieve primes up to the computed limit */
    idx_t *tmp = NULL;
    size_t tmpcnt = 0;
    sieve_primes(limit, &tmp, &tmpcnt);

    /* Replace the old cache */
    if (prime_cache)
        free(prime_cache);
    prime_cache = tmp;
    prime_cache_cnt = tmpcnt;
    prime_cache_limit = limit;
}

static idx_t nth_prime(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but avoid UB */
    extend_prime_cache_to((size_t)n);
    return prime_cache[n - 1];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* offset starts at 1, undefined otherwise */

    idx_t p1 = nth_prime(n);
    idx_t p2 = nth_prime(p1 - 1);
    idx_t p3 = nth_prime(p2 - 1);
    return (seq_t)p3 - 1;
}
