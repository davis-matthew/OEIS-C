/*
 * A046886  Number of divisors d of 2n satisfying (d+1) = prime or number of
 *          prime factors of the denominator of the even Bernoulli numbers.
 * https://oeis.org/A046886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p ≤ 2n+1 such that (p‑1) divides 2n
 *            (equivalently, a(n) is the number of prime factors of the
 *            denominator of the even Bernoulli number B_{2n})
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* 2n fits in idx_t (unsigned 64‑bit) for all realistic n. */
    idx_t two_n = n * 2ULL;
    idx_t limit  = two_n + 1ULL;          /* maximum prime needed */

    uint64_t *primes = NULL;
    size_t   prime_cnt = 0;
    /* Build list of all primes ≤ limit.  build_primes_upto allocates the array. */
    build_primes_upto(limit, &primes, &prime_cnt);

    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        /* (p-1) always fits in idx_t because p ≤ limit ≤ 2n+1. */
        if (two_n % ((idx_t)(p) - 1ULL) == 0) {
            ++count;
        }
    }

    /* Free the allocated prime array. */
    free(primes);
    return count;
}
