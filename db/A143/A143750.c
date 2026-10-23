/*
 * A143750  a(n)=10^C(n,2)*(1-10^n)/9.
 * https://oeis.org/A143750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 10^C(n,2)*(1-10^n)/9
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 4, i64 n <= 5, i128 n <= 8,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* compute C(n,2) safely: for n<2 the value is 0 */
    idx_t c = 0;
    if (n >= 2) {
        c = n * (n - 1) / 2;
    }

    /* 10^C(n,2) and 10^n */
    seq_t powC = pow10(c);
    seq_t powN = pow10(n);

    /* repunit (10^n - 1) / 9 */
    seq_t repunit = (powN - 1) / 9;

    /* a(n) = -10^C(n,2) * repunit */
    return -powC * repunit;
}
