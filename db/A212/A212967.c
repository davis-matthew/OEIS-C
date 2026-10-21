/*
 * A212967  Number of (w,x,y) with all terms in {0,...,n} and w <
 *          range{w,x,y}.
 * https://oeis.org/A212967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)*(10*n*(n + 2) - 3*(-1)^n + 3)/24
 * Range:     u8 n <= 4, u16 n <= 52, u32 n <= 1003, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;     /* cast index to seq_t first */
    seq_t n1  = N + 1;        /* n + 1          */
    seq_t term;

    /* use the more explicit piecewise form to avoid the (-1)^n term */
    if ((n & 1U) == 0) {                /* n even */
        term = (seq_t)10 * N * (N + 2);
    } else {                            /* n odd  */
        term = (seq_t)10 * N * (N + 2) + 6;
    }

    /* the division by 24 yields an integer result */
    return (n1 * term) / 24;
}
