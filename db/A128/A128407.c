/*
 * A128407  Triangle read by rows: T(n,n) = mobius(n) on the diagonal, zero
 *          elsewhere.
 * https://oeis.org/A128407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign, easy
 * Formula:   a(n) = mu(k) if n = k*(k+1)/2, otherwise 0
 * Range:     i8 n <= 7, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 968 (2026-11-01)
 * Helpers:   ipow_i32, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mobius.h"

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

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset starts at 1, but guard against 0 */

    /* d = 8*n + 1 */
    seq_t d = (seq_t)n * 8 + 1;

    /* s = floor(sqrt(d)) */
    seq_t s = floor_sqrt(d);

    /* Check if d is a perfect square and s is odd:
       then n is a triangular number with index k = (s-1)/2. */
    if (s * s == d && (s & 1)) {
        idx_t k = (idx_t)((s - 1) / 2);
        return mobius(k);
    }

    return 0;
}
