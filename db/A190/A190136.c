/*
 * A190136  Largest prime factor of n*(n+1)*(n+2)*(n+3).
 * https://oeis.org/A190136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{gpf(n), gpf(n+1), gpf(n+2), gpf(n+3)}
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9406 (2026-11-01)
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the utility function gpf from ../utils/gpf.h */

#include <stdint.h>
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    uint64_t np = (uint64_t)n;
    uint64_t maxp = 0;

    for (int i = 0; i < 4; ++i)
    {
        uint64_t p = largest_prime_factor(np + i);
        if (p > maxp)
            maxp = p;
    }

    return (seq_t)maxp;
}
