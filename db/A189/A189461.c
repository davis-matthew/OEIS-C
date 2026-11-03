/*
 * A189461  n+[nr/s]+[nt/s]; r=2, s=sqrt(5), t=1+sqrt(5).
 * https://oeis.org/A189461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + floor(n/√5) + floor(2n/√5)
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 7074, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9832 (2026-11-01)
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
    if (n == 0) return 0;         /* offset is 1; value for n<1 is irrelevant */

    seq_t nn = (seq_t)n;

    /* floor(n/√5)  = floor( floor(√(5*n^2)) / 5 ) */
    seq_t t1 = floor_sqrt( (seq_t)5 * nn * nn );
    seq_t f_n = t1 / (seq_t)5;

    /* floor(2n/√5) = floor( floor(√(20*n^2)) / 5 ) */
    seq_t t2 = floor_sqrt( (seq_t)20 * nn * nn );
    seq_t f_2n = t2 / (seq_t)5;

    return (seq_t)2 * nn + f_n + f_2n;
}
