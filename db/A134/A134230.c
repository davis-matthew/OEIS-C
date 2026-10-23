/*
 * A134230  a(n) = (10^n+1)^2 - 1.
 * https://oeis.org/A134230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10^n + 1)^2 - 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t p = pow10(n);           /* 10^n */
    seq_t t = p + (seq_t)1;       /* 10^n + 1 */
    seq_t res = t * t;            /* (10^n + 1)^2 */
    res -= (seq_t)1;              /* - 1 */
    return res;
}
