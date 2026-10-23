/*
 * A144065  Values of k such that the expression sqrt(4!*(k+1) + 1) yields an
 *          integer.
 * https://oeis.org/A144065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-3 + 2*(-1)^n*n + 3*(-1)^n + 6*n^2 + 18*n)/16
 * Range:     u8 n <= 24, u16 n <= 416, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* use signed 64‑bit arithmetic for the intermediate computation */
    int64_t nn   = (int64_t)n;
    int64_t sgn  = (nn & 1) ? -1 : 1;
    int64_t val = (-3 + 3*sgn + 2*sgn*nn + 6*nn*nn + 18*nn) / 16;
    return (seq_t)val;
}
