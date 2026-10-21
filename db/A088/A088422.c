/*
 * A088422  Number of primes in arithmetic progression starting with 7 and
 *          with d=2n.
 * https://oeis.org/A088422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A088422: a(n) = number of primes in the arithmetic progression
   7, 7+2n, 7+2·2n, … until the first non‑prime occurs. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;                     /* number of primes found */
    uint64_t d = 2ULL * n;               /* common difference = 2n */

    uint64_t k = 0;                      /* current term index (k = 0 gives 7) */
    for (;; ++k)
    {
        /* compute 7 + k*d using 128‑bit arithmetic to avoid overflow */
        __uint128_t term128 = (__uint128_t)7 + (__uint128_t)k * d;

        /* if the term would not fit in 64 bits we stop – values beyond 2^64
           do not occur for the n-range used in the tests */
        if (term128 > (uint64_t)UINT64_MAX)
            break;

        uint64_t term = (uint64_t)term128;

        /* check primality */
        if (is_prime(term))
            ++count;            /* prime – continue counting */
        else
            break;              /* first composite – stop */
    }

    return count;
}
