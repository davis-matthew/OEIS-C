/*
 * A190483  a(n) = [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(2),2,1) and
 *          []=floor.
 * https://oeis.org/A190483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2n+1)√2) - 2·floor(n√2) - 1 * Computation uses
 *            integer square roots: floor(n√2) = floor(√(2n²))
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 130 terms through n = 968 (2026-11-01)
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
    /* Convert n to the base integer type for arithmetic */
    seq_t nn = (seq_t)n;          /* n as seq_t */

    /* floor(n√2) = floor(√(2 n²)) */
    seq_t t1 = nn * nn * (seq_t)2;       /* 2 * n^2   */
    seq_t floor_n = floor_sqrt(t1);

    /* floor((2n+1)√2) = floor(√(2 (2n+1)²)) */
    seq_t m = nn * (seq_t)2 + (seq_t)1;  /* m = 2n+1   */
    seq_t t2 = m * m * (seq_t)2;         /* 2 * (2n+1)^2 */
    seq_t floor_m = floor_sqrt(t2);

    /* Final formula */
    seq_t result = floor_m - (seq_t)2 * floor_n - (seq_t)1;
    return result;
}
