/*
 * A230038  Distance between n^2 and the smallest triangular number >= n^2.
 * https://oeis.org/A230038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest triangular number ≥ n² minus n²
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 98 terms through n = 978 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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
    if (n == 0) return (seq_t)0;           /* safety for n < 1 */

    seq_t nseq = (seq_t)n;
    seq_t m2   = nseq * nseq;              /* n² */

    /* compute ceil( (-1 + sqrt(1 + 8 n²)) / 2 ) */
    seq_t val = (seq_t)1 + (seq_t)8 * m2;
    seq_t s   = floor_sqrt(val);           /* floor sqrt of val */

    seq_t k = (s - (seq_t)1) / (seq_t)2;    /* initial guess */

    /* verify and correct if necessary */
    seq_t Tk = k * (k + (seq_t)1) / (seq_t)2;
    if (Tk < m2) {
        ++k;
        Tk = k * (k + (seq_t)1) / (seq_t)2;
    }

    return Tk - m2;
}
