/*
 * A184738  floor(nr+h), where r=-1+sqrt(5), h=1/2; complement of A184735.
 * https://oeis.org/A184738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(-1+sqrt(5))+1/2)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the identity a(n) = floor(sqrt(5*n*n)) - n + [fractional part of sqrt(5*n*n) >= 1/2] */
/* Terms for n >= 1.                                  */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* value for n < 1 is irrelevant */

    seq_t nn = (seq_t)n;

    /* compute floor(sqrt(5*n*n)) without overflow for n up to ~30 million   */
    seq_t sq   = (seq_t)5 * nn * nn;
    seq_t m    = floor_sqrt(sq);

    /* indicator whether fractional part of sqrt(5*n*n) is at least 1/2      */
    seq_t two_m_plus1 = m * 2 + 1;            /* 2*m + 1   */
    seq_t rhs    = two_m_plus1 * two_m_plus1;/* (2*m+1)^2  */
    seq_t lhs    = (seq_t)20 * nn * nn;       /* 20*n*n    */
    seq_t inc    = (lhs >= rhs) ? 1 : 0;      /* 1 if >= 1/2, else 0 */

    return m - nn + inc;
}
