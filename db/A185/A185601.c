/*
 * A185601  a(n) = floor(n^(3/2))-floor(n^(1/2))-n; complement of A185602.
 * https://oeis.org/A185601
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^(3/2)) - floor(n^(1/2)) - n
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 555, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 94 terms through n = 4599 (2026-11-01)
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
    /* The sequence is defined for n >= 3; for smaller n the value is irrelevant. */
    if (n < 3)
        return 0;

    seq_t nseq = (seq_t)n;
    /* floor(n^(3/2)) == floor(sqrt(n^3)) */
    seq_t n2   = nseq * nseq;          /* n^2   */
    seq_t n3   = n2   * nseq;          /* n^3   */
    seq_t fn3  = floor_sqrt(n3);       /* floor sqrt(n^3) */

    /* floor(n^(1/2)) */
    seq_t fn1  = floor_sqrt(nseq);      /* floor sqrt(n) */

    return fn3 - fn1 - nseq;
}
