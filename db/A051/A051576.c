/*
 * A051576  Order of Burnside group B(3,n) of exponent 3 and rank n.
 * https://oeis.org/A051576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3^(n*(n^2+5)/6)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Compute the exponent:  n*(n^2+5)/6  */
    idx_t squares = n * n;                    /* n^2  */
    idx_t exp     = n * (squares + 5) / 6;    /* integer exponent */

    return pow3(exp);                          /* 3^exp */
}
