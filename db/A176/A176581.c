/*
 * A176581  n^3+Smallest square, (Smallest square >= n^3).
 * https://oeis.org/A176581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3 + ceil(sqrt(n^3))^2
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 873, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9106 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* use the pre‑built integer square root helper */
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
    /* cast the index to the sequence type early */
    seq_t nn = (seq_t)n;

    /* compute n^3 */
    seq_t n3 = nn * nn * nn;

    /* get floor(sqrt(n^3)) */
    seq_t k  = floor_sqrt(n3);

    /* k*k is the greatest perfect square ≤ n^3 */
    seq_t sq = k * k;

    if (sq == n3)            /* n^3 itself is a perfect square */
    {
        /* smallest square ≥ n^3 is n^3 itself */
        return n3 + sq;
    }
    else
    {
        /* next integer after the floor square root */
        seq_t kp1 = k + 1;
        seq_t sq1 = kp1 * kp1;   /* ceil(sqrt(n^3))^2 */

        return n3 + sq1;
    }
}
