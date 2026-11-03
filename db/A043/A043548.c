/*
 * A043548  Least separator of first n Egyptian fractions; i.e., least k for
 *          which the integers floor(k/m) for m=1,2,...,n are distinct.
 * https://oeis.org/A043548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 - n*t + floor(t^2/4), with t = floor(sqrt(4*n-3))
 * Range:     u8 n <= 19, u16 n <= 271, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 954 (2026-11-01)
 * Helpers:   ipow_i32, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer-computation using floor sqrt and rounded sqrt. */
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/isqrt.h"

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
    /* OEIS offset: a(1) = 1 */
    if (n <= 1)
        return (seq_t)1;

    seq_t nseq = (seq_t)n;

    /* floor(sqrt(n-1)) */
    seq_t s1 = floor_sqrt(nseq - 1);

    /* rounded sqrt: floor(sqrt(n)+1/2) */
    idx_t r = isqrt(n);
    idx_t threshold = r * r + r + 1;   /* threshold for rounding up */
    seq_t s2;
    if (n >= threshold)
        s2 = (seq_t)(r + 1);
    else
        s2 = (seq_t)r;

    /* a(n) = n^2 - n*(s1 + s2) + s1*s2 */
    return nseq * nseq - nseq * (s1 + s2) + s1 * s2;
}
