/*
 * A183862  a(n) = n + floor(sqrt(5*n/2)); complement of A183863.
 * https://oeis.org/A183862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(5*n/2))
 * Range:     u8 n <= 25, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-01)
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

/* n has to be >= 1 (offset 1). */
seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;           /* cast index */
    seq_t t;                      /* floor(5*n/2) */
    if ((n & 1) == 0) {
        /* even n: 5*n/2 = (n/2)*5 */
        t = (m / 2) * (seq_t)5;
    } else {
        /* odd n: 5*n/2 = (n-1)/2*5 + 2 */
        t = ((m - 1) / 2) * (seq_t)5 + (seq_t)2;
    }
    seq_t s = floor_sqrt(t);
    return m + s;
}
