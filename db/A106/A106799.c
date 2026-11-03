/*
 * A106799  Number of prime factors of n apart from 2 or 3, counted with
 *          multiplicity.
 * https://oeis.org/A106799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime factors of n except 2 and 3, counted with
 *            multiplicity. Implementation: strip all factors of 2 and 3 from
 *            n, then compute the total number of remaining prime factors
 *            (with multiplicity) using the helper bigomega_uint64
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 8533 (2026-11-01)
 * Helpers:   bigomega_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega_u64.h"

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    while ((x & 1U) == 0)         /* remove factor 2 */
        x >>= 1;
    while (x % 3U == 0)          /* remove factor 3 */
        x /= 3U;
    /* the remaining x contains only primes > 3 (or 1). */
    return bigomega_u64(x);
}
