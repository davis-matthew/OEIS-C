/*
 * A136483  Number of unit square lattice cells inside quadrant of
 *          origin-centered circle of diameter n.
 * https://oeis.org/A136483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} floor(sqrt((n/2)^2 - k^2))
 * Range:     u8 n <= 8, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 988 (2026-11-01)
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
    /* sequence is defined for n ≥ 1; for n = 0 return 0 safely */
    if (n == 0) return 0;

    seq_t sum = 0;
    idx_t limit = n / 2;                     /* floor(n/2) */

    seq_t nseq = (seq_t)n;
    seq_t n2   = nseq * nseq;                /* n^2  (fits in seq_t for given limits) */

    for (idx_t k = 1; k <= limit; ++k) {
        seq_t kseq = (seq_t)k;
        seq_t k2   = kseq * kseq;
        /* A = n^2 - 4*k^2  is always non‑negative for k ≤ floor(n/2) */
        seq_t A    = n2 - (seq_t)4 * k2;
        seq_t fs   = floor_sqrt(A);          /* floor(sqrt(A)) */
        sum += fs >> 1;                      /* floor(fs / 2) = floor(sqrt(A)/2) */
    }

    return sum;
}
