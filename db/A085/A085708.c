/*
 * A085708  Arithmetic derivative of 10^n.
 * https://oeis.org/A085708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n*10^(n-1) (with a(0) = 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t res = pow10(n - 1);   /* 10^(n-1) */
    res *= (seq_t)n;            /* multiply by n */
    res *= (seq_t)7;            /* multiply by 7 */
    return res;
}
