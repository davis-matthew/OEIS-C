/*
 * A118486  a(n) is the smallest prime occurring in the prime factorization of
 *          n! to an odd power.
 * https://oeis.org/A118486
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime p such that p appears with odd
 *            exponent in the prime factorisation of n!. This is equivalent to
 *            the smallest prime factor of C(n, n/2) when n is even
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes_cache = NULL; /* cache of primes up to the largest n seen */
static size_t  prime_count   = 0;
static idx_t   primes_limit  = 0;     /* current limit of cached primes */

seq_t a(idx_t n)
{
    if (n < 2) {                     /* behavior below the offset is irrelevant */
        return 0;
    }

    /* Extend prime cache if necessary */
    if (n > primes_limit) {
        if (primes_cache) {
            free(primes_cache);
            primes_cache = NULL;
        }
        build_primes_upto(n, &primes_cache, &prime_count);
        primes_limit = n;
    }

    /* Find the first prime ≤ n with an odd exponent in n! */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes_cache[i];
        if (p > n) break;           /* no need to check larger primes */

        idx_t temp = n;
        idx_t parity = 0;           /* 0 even, 1 odd */
        while (temp) {
            temp /= p;
            parity ^= (temp & 1);
        }
        if (parity) {                /* odd exponent found */
            return (seq_t)p;
        }
    }

    /* Fallback – should never happen for n ≥ 2 */
    return 0;
}
