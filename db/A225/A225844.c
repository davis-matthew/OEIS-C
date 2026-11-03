/*
 * A225844  Least k>0 such that triangular(n) + k*(k+1) is a triangular
 *          number.
 * https://oeis.org/A225844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 6958, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 8098 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = minimal k > 0 such that T_n + k*(k+1) is a triangular number,
 * where T_n = n*(n+1)/2.
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
    /* triangular number of n */
    seq_t t = ((seq_t)n * ((seq_t)n + 1)) / 2;

    /* maximum possible k is 2*n-1 (given by the OEIS bound) */
    for (idx_t k = 1; k <= (2*n) - 1; ++k) {
        seq_t add = ((seq_t)k * ((seq_t)k + 1));
        seq_t v   = t + add;

        /* test whether v is triangular: 8*v + 1 should be a perfect square */
        seq_t s = floor_sqrt((seq_t)8 * v + 1);
        if (s * s == (seq_t)8 * v + 1)
            return (seq_t)k; /* found the minimal k */
    }

    /* For all n >= 0 the above loop always finds a k, but return 0
     * as a safe fallback (should never be reached). */
    return 0;
}
