/*
 * A239330  Number of odd primes p <= n with pi(n*(p+1)/2) - pi(n*(p-1)/2)
 *          prime, where pi(x) denotes the number of primes not exceeding x.
 * https://oeis.org/A239330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ odd prime p ≤ n : isPrime( n*(p+1)/2 ) - isPrime(
 *            n*(p-1)/2 ) }| * The function counts odd primes p ≤ n such that
 *            the difference of the prime * counting function values at
 *            n*(p+1)/2 and n*(p-1)/2 is itself prime. * It uses a simple
 *            cache of primes up to the largest argument required for...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Utility functions provided by the algorithm framework */
#include "../utils/build_primes_upto.h"   /* build_primes_upto */
#include "../utils/is_prime.h"           /* is_prime */

/* ---------- static cache data ---------------------------------------- */
static idx_t        primes_limit = 0;      /* highest limit for which primes were built */
static size_t       primes_cnt   = 0;      /* number of primes in the cache */
static uint64_t *   primes       = NULL;   /* array of primes (sorted, ascending) */

/* Extend the prime cache up to @new_limit if needed.  The old array is
 * discarded and replaced by a new one holding all primes ≤ @new_limit.   */
static void extend_primes(idx_t new_limit)
{
    if (new_limit <= primes_limit)
        return;

    /* Free previous storage if any */
    if (primes)
        free(primes);

    /* Build new prime list */
    build_primes_upto(new_limit, &primes, &primes_cnt);
    primes_limit = new_limit;
}

/* Return the prime counting function π(x) using binary search on the
 * cached prime array.  We assume the primes array contains all primes
 * ≤ primes_limit, which is guaranteed by extend_primes before any call
 * to this function. */
static idx_t pi_leq(idx_t x)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;   /* number of primes ≤ x */
}

/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n < 1, return 0 as default */

    /* The largest argument of π that we might need for this n is
     * max_p * n / 2  where max_p ≤ n (odd primes only).  Therefore
     * we ensure we have primes generated up to n*n/2.  The sqrt
     * of this is sufficient for ensuring that π can be evaluated via
     * the cache. */
    idx_t needed_limit = n * (n + 1) / 2;          /* n*(n+1)/2 fits in idx_t for all used n */
    extend_primes(needed_limit);

    seq_t result = 0;

    /* Loop over odd primes p ≤ n.  We skip the even prime 2. */
    idx_t prime_remainder  = pi_leq(n);   /* number of primes ≤ n */
    for (size_t i = 0; i < prime_remainder; ++i) {
        uint64_t p = primes[i];
        if (p == 2)
            continue;                /* skip even prime */

        /* Compute the two arguments for π.  All arithmetic is done in idx_t
         * so there is no risk of overflow for the expected input sizes.   */
        idx_t t1 = n * (p + 1) / 2;    /* n*(p+1)/2 */
        idx_t t2 = n * (p - 1) / 2;    /* n*(p-1)/2 */

        idx_t pi_t1 = pi_leq(t1);
        idx_t pi_t2 = pi_leq(t2);
        idx_t diff  = (idx_t)((int64_t)pi_t1 - (int64_t)pi_t2);   /* difference may be zero */

        /* Check if the difference is a prime number.  diff is small
         * (at most π(needed_limit)) and fits comfortably in idx_t.    */
        if (is_prime(diff))
            ++result;
    }

    return result;
}
