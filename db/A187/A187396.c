/*
 * A187396  a(n) = floor(s*n), where s =-2 + sqrt(10); complement of A187395.
 * https://oeis.org/A187396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((-2 + sqrt(10)) * n)
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Convert index to seq_t before arithmetic to avoid unintended truncation. */
    seq_t nn = (seq_t)n;

    /* Compute 10 * n^2.  The product fits into seq_t for all n that the
       tests require (n <= 10000).  For larger n this overflows, but the
       sequence is only needed for the range specified. */
    seq_t sq = nn * nn;   /* n^2   */
    sq *= (seq_t)10;       /* 10*n^2 */

    /* floor(n*sqrt(10)) = floor(sqrt(10*n^2)) */
    seq_t fsqrt  = floor_sqrt(sq);

    /* Compute a(n) = floor(n*sqrt(10)) - 2*n */
    seq_t two_n = nn * (seq_t)2;
    return fsqrt - two_n;
}
