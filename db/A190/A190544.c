/*
 * A190544  a(n) = [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(2),4,0) and
 *          []=floor.
 * https://oeis.org/A190544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(4*n*sqrt(2)) - 4*floor(n*sqrt(2))
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 144 terms through n = 978 (2026-11-01)
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
    /* Convert index to seq_t once for reuse */
    seq_t N = (seq_t)n;

    /* Compute floor(n*sqrt(2)) via: floor(sqrt(2*n^2)) */
    seq_t t1 = (seq_t)2 * N * N;
    seq_t floor_n = floor_sqrt(t1);

    /* Compute floor(4*n*sqrt(2)) via: floor(sqrt(32*n^2)) */
    seq_t t2 = (seq_t)32 * N * N;
    seq_t floor_4n = floor_sqrt(t2);

    /* Final result: floor_4n - 4*floor_n */
    return floor_4n - (seq_t)4 * floor_n;
}
