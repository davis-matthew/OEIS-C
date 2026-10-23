/*
 * A037156  a(n) = 10^n*(10^n+1)/2.
 * https://oeis.org/A037156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^n*(10^n+1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute p = 10^n */
    seq_t p = pow10(n);

    /* Compute p*(p+1)/2. Do the division before the multiplication
       when possible to keep intermediate values smaller.      */
    seq_t res;
    if ((p & 1) == 0)        /* p is even */
        res = (p >> 1) * (p + 1);
    else                     /* p+1 is even */
        res = p * ((p + 1) >> 1);

    return res;
}
