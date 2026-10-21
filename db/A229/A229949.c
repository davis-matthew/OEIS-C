/*
 * A229949  Number of divisors of the n-th positive quarter-square.
 * https://oeis.org/A229949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000005( A002620(n+1) ) = number of divisors of floor(
 *            (n+1)^2 / 4 )
 * Range:     u8 n <= 30, u16 n <= 1493, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"   /* provides uint64_t divisor_count_u64(uint64_t) */

seq_t a(idx_t n)
{
    /* m = n + 1, use idx_t to avoid overflow */
    idx_t m = n + (idx_t)1;

    /* Compute floor(m*m/4) in seq_t */
    seq_t mm = (seq_t)m;
    seq_t val = (mm * mm) / (seq_t)4;

    /* Convert to uint64_t for divisor counting; the value fits into 64‑bit
       for the range of n used in the tests. */
    uint64_t uval = (uint64_t)val;

    /* Number of divisors (A000005) */
    uint64_t d = divisor_count_u64(uval);

    return (seq_t)d;
}
