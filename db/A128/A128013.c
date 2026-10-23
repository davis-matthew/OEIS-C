/*
 * A128013  a(n) = (n^3 +n)*5^n.
 * https://oeis.org/A128013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + n) * 5^n ; offset 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 47,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"

seq_t a(idx_t n)
{
    /* cast n into seq_t so all arithmetic is performed in seq_t */
    seq_t sn   = (seq_t)n;
    seq_t n_sq = sn * sn;                     /* n^2 */
    seq_t fac  = sn * (n_sq + (seq_t)1);      /* n * (n^2 + 1) = n^3 + n */
    seq_t pow5 = power5(n);                   /* 5^n */
    return fac * pow5;
}
