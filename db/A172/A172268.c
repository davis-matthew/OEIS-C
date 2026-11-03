/*
 * A172268  a(n) = floor(n*(sqrt(7)-sqrt(3))).
 * https://oeis.org/A172268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(sqrt(7)-sqrt(3)))
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 468, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 89 terms through n = 981 (2026-11-01)
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
    /* Convert the index to the arithmetic type */
    seq_t nn = (seq_t)n;

    /* n^2 */
    seq_t n2 = nn * nn;

    /* floor(n*sqrt(7)) and floor(n*sqrt(3)) via integer square root */
    seq_t s7 = floor_sqrt(n2 * (seq_t)7);
    seq_t s3 = floor_sqrt(n2 * (seq_t)3);

    /* Initial candidate for the sequence value */
    seq_t M = s7 - s3;

    /* L = 4*n^2 - M^2  (always positive) */
    seq_t L = (seq_t)4 * n2 - M * M;

    /* left = L^2 */
    seq_t left = L * L;

    /* right = 12*M^2*n^2  */
    seq_t right = ((seq_t)12 * M) * M * n2;

    /* The sequence value equals M or M-1 depending on the inequality */
    if (left < right)
        M = M - (seq_t)1;

    return M;
}
