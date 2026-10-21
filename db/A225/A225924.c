/*
 * A225924  Number of solutions to 2n - 1 = (p1*p2 + p3)/2, where p1, p2, and
 *          p3 are prime numbers less than 2n-1.
 * https://oeis.org/A225924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1957, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = # of unordered prime triples (p1,p2,p3) of primes < 2n-1
 * satisfying 2n-1 = (p1*p2 + p3)/2 .
 *
 * The implementation builds a list of primes up to 2n-1 once per maximum
 * n encountered and counts all unordered pairs (p1<=p2).  For each pair it
 * checks if the remaining value p3 is also a prime < 2n-1.
 *
 * The algorithm is O(pi(2n))^2 in the worst case which is fine for n up to
 * at least 10 000 (pi(20000) ≈ 2267).  The sieve uses the provided
 * build_primes_upto helper.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Binary search to test whether x is in the sorted prime array. */
static bool prime_in_list(const uint64_t *primes, size_t cnt, uint64_t x)
{
    size_t lo = 0, hi = cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < x)
            lo = mid + 1;
        else if (primes[mid] > x)
            hi = mid;
        else
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* --- static cache of primes up to current maximum limit --- */
    static uint64_t *prime_cache = NULL;
    static size_t prime_cnt = 0;
    static idx_t built_limit = 0;   /* last limit for which primes are built */

    /* Bail out for trivial n.  For n=1 the limit is 1 and there are no primes. */
    if (n < 2)
        return (seq_t)0;

    /* Compute the limit 2n-1 and the target 4n-2. */
    idx_t limit  = (idx_t)2 * n - 1;
    idx_t target = (idx_t)4 * n - 2;   /* equals 2*(2n-1) */

    /* Rebuild prime list if we see a larger limit than before. */
    if ((idx_t)limit > built_limit) {
        /* Free previous cache, if any. */
        free(prime_cache);
        prime_cache = NULL;
        prime_cnt   = 0;

        /* Allocate up to the new limit.  build_primes_upto expects the upper
         * bound to be inclusive; we need primes < limit, so use limit-1.  */
        if (limit > 1)
            build_primes_upto((idx_t)limit - 1, &prime_cache, &prime_cnt);
        else
            prime_cnt = 0;              /* no primes less than 2 */

        built_limit = limit;
    }

    seq_t count = 0;

    /* Iterate over unordered prime pairs (p1 <= p2). */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p1 = prime_cache[i];
        if (p1 * p1 >= target)  /* product already too large for i=i case */
            break;
        for (size_t j = i; j < prime_cnt; ++j) {
            uint64_t p2 = prime_cache[j];
            /* Stop inner loop if product exceeds target-2 (since p3 must be ≥ 2). */
            if (p1 * p2 >= target)
                break;
            uint64_t p3 = target - p1 * p2;
            if (p3 >= (uint64_t)limit)   /* p3 must be < 2n-1 */
                continue;
            if (prime_in_list(prime_cache, prime_cnt, p3))
                ++count;
        }
    }

    return count;
}
