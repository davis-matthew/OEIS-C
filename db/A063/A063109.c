/*
 * A063109  Dimension of the space of weight 2n cusp forms for Gamma_0( 41 ).
 * https://oeis.org/A063109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((-1)^n + 14*n - 9) / 2 for n > 1, with a(1) = 3
 * Range:     u8 n <= 37, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;

    seq_t sn = (seq_t)n;
    /* For n even: a(n) = 7*n - 4
       For n odd : a(n) = 7*n - 5  */
    if (n & 1ULL)   /* odd */
        return sn * (seq_t)7 - (seq_t)5;
    else            /* even */
        return sn * (seq_t)7 - (seq_t)4;
}
