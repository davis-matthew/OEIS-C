/*
 * A184624  a(n) = floor(n*r +h), where r=sqrt(2), h=-1/4; complement of
 *          A184619.
 * https://oeis.org/A184624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(2) - 1/4)
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n >= 1; compute floor(n*sqrt(2)) as floor_sqrt(2*n^2) */
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;            /* n^2 */
    seq_t two_n2 = n2 << 1;              /* 2*n^2 */
    seq_t m = floor_sqrt(two_n2);        /* floor(n*sqrt(2)) */

    /* Determine whether the fractional part of n*sqrt(2) is < 1/4.
       The condition is 32*n^2 >= 16*m^2 + 8*m + 1. */
    seq_t left  = n2 << 5;               /* 32*n^2 */
    seq_t m2    = m * m;                 /* m^2 */
    seq_t right = (m2 << 4) + (m << 3) + 1; /* 16*m^2 + 8*m + 1 */

    return (left >= right) ? m : (m - 1);
}
