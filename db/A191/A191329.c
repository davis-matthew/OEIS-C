/*
 * A191329  (Lower Wythoff sequence mod 2)+(Upper Wythoff sequence mod 2).
 * https://oeis.org/A191329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (floor(n*phi) mod 2) + ((floor(n*phi) + n) mod 2)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 148 terms through n = 9113 (2026-11-01)
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
    /* compute floor(n*phi) exactly using the identity
       floor(n*phi) = floor((n + floor(n*sqrt(5)))/2)                     */
    if (n == 0) return 0; /* undefined, but safe */

    seq_t N = (seq_t)n;
    seq_t v = (seq_t)5 * N * N;            // v = 5 * n^2
    seq_t sq = floor_sqrt(v);              // floor(sqrt(5 * n^2)) = floor(n*sqrt5)
    seq_t u = (N + sq) / 2;                // floor(n*phi)

    unsigned uParity = (unsigned)(u & 1);  /* parity of floor(n*phi)     */
    unsigned nParity = (unsigned)(N & 1);  /* parity of n                */

    /* a(n) = uParity + (uParity ^ nParity)   */
    seq_t result = (seq_t)(uParity + (uParity ^ nParity));
    return result;
}
