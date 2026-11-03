/*
 * A175608  Characteristic function of squarefree triangular integers: 1 if
 *          n(n+1)/2 is squarefree else 0.
 * https://oeis.org/A175608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008966(n*(n+1)/2)
 * Range:     u8 n <= 10, u16 n <= 105, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9739 (2026-11-01)
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
    /* Compute the n‑th triangular number. */
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;

    /* If any prime square divides t, the number is not square‑free. */
    seq_t limit = floor_sqrt(t);

    for (idx_t i = 2; (seq_t)i <= limit; ++i) {
        seq_t sq = (seq_t)i * (seq_t)i;
        if (t % sq == 0)
            return 0;
    }

    return 1;
}
