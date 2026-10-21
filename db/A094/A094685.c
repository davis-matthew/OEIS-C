/*
 * A094685  Modified juggler sequence: if n mod 2 = 0 then round(sqrt(n)) else
 *          round(n^(3/2)).
 * https://oeis.org/A094685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(sqrt(n)) for n even, * round(n^(3/2)) for n odd
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 3112, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if ((n & 1) == 0) {                /* n is even */
        seq_t nn = n;
        seq_t k   = floor_sqrt(nn);    /* floor(sqrt(n)) */
        seq_t lhs = nn * 4;            /* 4*n */
        seq_t tmp = (seq_t)(2 * k + 1);
        seq_t rhs = tmp * tmp;         /* (2*k+1)^2 */
        return (lhs >= rhs) ? k + 1 : k;
    } else {                            /* n is odd */
        seq_t nn = n;
        seq_t m  = nn * nn * nn;       /* n^3 */
        seq_t k  = floor_sqrt(m);      /* floor(sqrt(n^3)) = floor(n*sqrt(n)) */
        seq_t lhs = m * 4;             /* 4*m */
        seq_t tmp = (seq_t)(2 * k + 1);
        seq_t rhs = tmp * tmp;         /* (2*k+1)^2 */
        return (lhs >= rhs) ? k + 1 : k;
    }
}
