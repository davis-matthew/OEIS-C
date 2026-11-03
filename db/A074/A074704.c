/*
 * A074704  a(n) = floor(n^(3/2)) - n*floor(n^(1/2)).
 * https://oeis.org/A074704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = floor(n^(3/2)) - n*floor(n^(1/2))
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 753, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 95 terms through n = 9714 (2026-11-01)
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
    /* Convert index to seq_t to avoid overflow in multiplications */
    seq_t N = n;

    /* floor(sqrt(n)) */
    seq_t s = floor_sqrt(N);

    /* floor(n^(3/2)) = floor(sqrt(n^3)) = floor_sqrt(N*N*N) */
    seq_t n3 = N * N * N;
    seq_t t = floor_sqrt(n3);

    /* result = floor(n^(3/2)) - n*floor(sqrt(n)) */
    return t - N * s;
}
