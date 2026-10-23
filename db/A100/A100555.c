/*
 * A100555  Smallest square that is equal to the sum of n
 *          not-necessarily-distinct primes plus 1.
 * https://oeis.org/A100555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (ceil(sqrt(2*n + 1)))^2
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A100555: the smallest square m² such that m²–1 is the
 * sum of n (not necessarily distinct) primes.  The minimal m is the
 * smallest integer with m² ≥ 2n + 1. */
#include <stdbool.h>
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    /* 2n + 1 fits in seq_t because n is uint64_t and seq_t is at least
     * 64 bits (or arbitrary precision). */
    seq_t val = (seq_t)n * 2 + 1;

    /* Integer square root: floor(sqrt(val)). */
    seq_t s   = isqrt(val);

    /* If s*s < val, increase s to get ceil(sqrt(val)). */
    if (s * s < val)
        ++s;

    /* Return the square of that integer. */
    return s * s;
}
