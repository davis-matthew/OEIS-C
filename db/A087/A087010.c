/*
 * A087010  Number of primes of form 4*k+1 between n and 2n (inclusive).
 * https://oeis.org/A087010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of primes p such that n ≤ p ≤ 2n and p ≡ 1 (mod 4)
 * Range:     u8 n <= 4524, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Compute the upper bound 2*n without overflow */
    idx_t limit;
    if (n > (UINT64_MAX >> 1))
        limit = UINT64_MAX;
    else
        limit = n << 1;

    seq_t count = 0;

    for (idx_t p = n; p <= limit; ++p)
    {
        /* Use the provided is_prime routine which accepts idx_t */
        if (is_prime(p) && (p & 3U) == 1U)
            ++count;
    }
    return count;
}
