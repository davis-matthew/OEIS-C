/*
 * A144111  Number of different pairs of primes p and q with p<q such that
 *          6*r(n)=p+q with r(i)=i-th prime.
 * https://oeis.org/A144111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime pairs (p , q) with p < q and p + q = 6 *
 *            r(n)
 * Range:     u8 n <= 14, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   build_primes_upto, get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where r(n) is the n-th prime and offset(1) == 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Guard against non‑positive indices.  The OEIS offset is 1. */
    if (n < 1)
        return (seq_t)0;

    /*------------------------------------------------------------------*
     * Step 1: compute the target sum 6 * r(n).  r(n) fits in a 32‑bit
     * unsigned integer for all n up to the test limit (≈ 10 000).  The
     * resulting target is therefore safely represented in a 64‑bit
     * unsigned integer.
     *------------------------------------------------------------------*/
    uint32_t prime_n = get_prime(n);                 /* r(n) */
    uint64_t target  = 6ULL * (uint64_t)prime_n;      /* desired sum */

    /* Only primes up to target/2 can be the smaller member of a pair. */
    uint64_t limit = target / 2ULL;

    /*------------------------------------------------------------------*
     * Step 2: maintain a global cache of all primes up to the largest
     * limit seen so far.  This makes repeated calls with arbitrary n
     * order efficient.
     *------------------------------------------------------------------*/
    static uint64_t *prime_list    = NULL; /* array of primes */
    static size_t   prime_count    = 0;    /* number of primes in array */
    static idx_t    prime_limit    = 0;    /* maximum value present */

    if ((idx_t)limit > prime_limit) {
        free(prime_list);
        prime_list  = NULL;
        prime_count = 0;
        prime_limit = 0;

        /* Build primes up to the required limit. */
        build_primes_upto((idx_t)limit, &prime_list, &prime_count);
        prime_limit = limit;
    }

    /*------------------------------------------------------------------*
     * Step 3: iterate over the cached primes, count qualifying pairs.
     *------------------------------------------------------------------*/
    seq_t count = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > limit)        /* p >= q, so stop */
            break;

        uint64_t q = target - p;
        if (p < q && is_prime_small((seq_t)q))
            ++count;
    }

    return count;
}
