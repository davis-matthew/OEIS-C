/*
 * A235887  Number of (n+1) X (2+1) 0..3 arrays with the minimum plus the
 *          upper median equal to the lower median plus the maximum in every 2
 *          X 2 subblock.
 * https://oeis.org/A235887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(6 + 9*2^n + 4^n)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t two_n  = pow2(n);          /* 2^n */
    seq_t four_n = pow2(2 * n);      /* 4^n = 2^(2n) */

    seq_t tmp = 6;
    tmp = tmp + (seq_t)9 * two_n;    /* 6 + 9*2^n */
    tmp = tmp + four_n;              /* + 4^n */
    return (seq_t)4 * tmp;           /* 4*(...) */
}
