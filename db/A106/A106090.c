/*
 * A106090  Prime numbers p such that 10p+7 and 7p+10 are primes.
 * https://oeis.org/A106090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A106090: primes p such that 10p + 7 and 7p + 10 are also prime.
   The function builds the sequence by scanning successive odd integers,
   testing primality with is_prime_uint64 (deterministic Miller–Rabin),
   and collecting the qualifying primes.  The list of terms is kept in a
   static array of fixed size (sufficient for n up to at least 10000; the
   behaviour for larger n is not required by the specification). */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    /* Fixed-size cache: large enough for the required range. */
    /* 20,000 elements comfortably covers the first 10,000 terms
       and keeps the array allocation simple for both C and C++. */
    #define CACHE_CAP 20000
    static seq_t cache[CACHE_CAP];
    static idx_t cacheCount = 0;   /* number of stored terms   */
    static uint64_t nextCandidate = 3;   /* next odd number to test */

    /* Extend the cache until it contains at least n terms. */
    while (cacheCount < n) {
        /* Find the next prime p starting from nextCandidate. */
        uint64_t p = nextCandidate;
        while (!is_prime_uint64(p))
            p += 2;        /* skip even numbers */

        /* Test the required condition: both 10p+7 and 7p+10 must be prime. */
        uint64_t v1 = 10ULL * p + 7ULL;
        uint64_t v2 = 7ULL *  p + 10ULL;
        if (is_prime_uint64(v1) && is_prime_uint64(v2)) {
            /* Store the qualifying prime.  The cache size is fixed, so
               we simply assume the required n will not exceed CACHE_CAP. */
            cache[cacheCount++] = (seq_t)p;
        }

        /* Next candidate is the next odd integer after p. */
        nextCandidate = p + 2;
    }

    return cache[n - 1];
}
