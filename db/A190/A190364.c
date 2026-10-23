/*
 * A190364  a(n) = n + [n*s/r] + [n*t/r] + [n*u/r]; r=sqrt(2), s=1/r,
 *          t=sqrt(3), u=1/t.
 * https://oeis.org/A190364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + [n/2] + [n*sqrt(3/2)] + [n*sqrt(1/6)]
 * Range:     u8 n <= 6, u16 n <= 74, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A190364.  The function is safe
 * for any n >= 1.  It relies on the integer square‑root routine
 * floor_sqrt(seq_t) which is provided by the framework. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* If called with n = 0, return 0 (outside the offset). */
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;          /* n as a seq_t  */

    seq_t result = nn;            /* start with n itself */

    /* floor(n/2)  */
    result += nn / 2U;

    /* floor( n * sqrt(3/2) )  = floor( sqrt( 3*n^2 / 2 ) ) */
    seq_t sq1_numer = (seq_t)3 * nn * nn;  /* 3*n^2 */
    seq_t sq1_div   = sq1_numer / 2U;      /* floor(3*n^2/2) */
    result += floor_sqrt(sq1_div);

    /* floor( n * sqrt(1/6) )  = floor( sqrt( n^2 / 6 ) ) */
    seq_t sq2_div = (nn * nn) / 6U;        /* floor(n^2/6) */
    result += floor_sqrt(sq2_div);

    return result;
}
