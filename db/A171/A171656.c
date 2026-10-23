/*
 * A171656  a(n) = 10^n*(7+3*10^n)+3.
 * https://oeis.org/A171656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^n*(7+3*10^n)+3
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t t  = pow10(n);      /* 10^n */
    seq_t t2 = t * t;         /* 10^(2n) */
    seq_t res = 3 * t2;       /* 3*10^(2n) */
    res += 7 * t;             /* +7*10^n */
    res += 3;                 /* +3 */
    return res;
}
