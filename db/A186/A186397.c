/*
 * A186397  Numbers k such that there are 5 primes between 100*k and 100*k +
 *          99.
 * https://oeis.org/A186397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Sequence A186397
 *  Numbers k such that there are exactly five primes between 100*k
 *  and 100*k+99 (inclusive).  The routine scans k in increasing order
 *  and counts primes in each 100‑wide block using a growing sieve.
 *
 *  a(n) returns the n‑th such k (1‑based).  The function is memoised:
 *  primes are generated once and reused for subsequent calls.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/*--- global state ------------------------------------------------------------------*/
static uint64_t *primes = NULL;   /* array of primes, sorted ascending          */
static size_t    prime_cnt = 0;   /* number of primes stored                      */
static idx_t     prime_limit = 0; /* maximum integer for which primes are known   */

/*--- helper: count primes in [low, high] ------------------------------------------------*/
static size_t count_primes_in_interval(uint64_t low, uint64_t high)
{
    /* binary search for first prime >= low */
    size_t left = 0, right = prime_cnt;
    while (left < right) {
        size_t mid = (left + right) >> 1;
        if (primes[mid] < low) left = mid + 1;
        else right = mid;
    }
    size_t lb = left;

    /* binary search for last prime <= high */
    left = 0; right = prime_cnt;
    while (left < right) {
        size_t mid = (left + right) >> 1;
        if (primes[mid] <= high) left = mid + 1;
        else right = mid;
    }
    size_t ub = left;               /* first index > high */
    return ub - lb;
}

/*--- helper: grow the prime table to cover at least 'limit' ---------------------------------*/
static void ensure_prime_limit(idx_t limit)
{
    if (prime_limit >= limit) return;
    /* if we already have some primes, reclaim memory and rebuild at larger size */
    if (primes) free(primes);
    build_primes_upto(limit, &primes, &prime_cnt);
    prime_limit = limit;
}

/*--- public routine -----------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 0 */

    /* initialise the prime table once on the first call */
    if (!prime_limit) {
        /* start with a moderately large limit; it will grow automatically */
        ensure_prime_limit(2000000ULL);
    }

    idx_t found = 0;
    for (idx_t k = 1; ; ++k) {
        uint64_t low  = (uint64_t)k * 100ULL;
        uint64_t high = low + 99ULL;

        /* grow sieve if the current interval lies beyond the known limit */
        if (high > (uint64_t)prime_limit) {
            idx_t new_limit = prime_limit * 2;
            if (new_limit < high + 10000ULL) new_limit = high + 10000ULL;
            ensure_prime_limit(new_limit);
        }

        if (count_primes_in_interval(low, high) == 5) {
            ++found;
            if (found == n) {
                return (seq_t)k;
            }
        }
    }
}
