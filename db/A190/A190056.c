/*
 * A190056  a(n) = n + [n*s/r] + [n*t/r]; r=1/2, s=sin(Pi/3), t=csc(Pi/3).
 * https://oeis.org/A190056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(3)) + floor(4*n/sqrt(3))
 * Range:     u8 n <= 3, u16 n <= 55, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9200 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses integer arithmetic and the utility floor_sqrt */

/* Include the integer sqrt utility */
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

/* Computes the OEIS sequence A190056 as: a(n) = n + floor(n*sqrt(3)) + floor(4*n/sqrt(3))  */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* sequence is defined for n >= 1; return 0 for safety */

    /* Convert n to seq_t once */
    seq_t n_val = (seq_t)n;

    /* floor(n * sqrt(3)) = floor(sqrt(3 * n^2))  */
    seq_t term1 = floor_sqrt((seq_t)3 * n_val * n_val);

    /* floor(4*n / sqrt(3)) = floor(sqrt((16/3) * n^2)) = floor_sqrt((16*n^2)/3)  */
    seq_t term2 = floor_sqrt(((seq_t)16 * n_val * n_val) / 3);

    return n_val + term1 + term2;
}
