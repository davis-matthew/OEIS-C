/*
 * A045914  Triangular numbers with all digits the same.
 * https://oeis.org/A045914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) = the nth triangular number that is a repdigit
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: full, pass, 7 terms through n = 7 (2026-11-02)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 1, a(3) = 3, a(4) = 6, a(5) = 55, ... */
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
    if (n == 1)
        return (seq_t)0;

    idx_t count = 1;          /* count includes the initial term 0 */
    seq_t pow10_k = 10;       /* 10^1 */
    for (idx_t k = 1; ; ++k)
    {
        for (int d = 1; d <= 9; ++d)
        {
            seq_t rep = (seq_t)d * (pow10_k - 1) / 9;

            /* Check whether rep is a triangular number: 1+8*rep must be a perfect square of odd integer */
            seq_t disc = 1 + (seq_t)8 * rep;
            seq_t s = floor_sqrt(disc);
            if (s * s == disc && (s & 1))
            {
                ++count;
                if (count == n)
                    return rep;
            }
        }
        pow10_k *= 10;   /* advance to next digit length */
    }
}
