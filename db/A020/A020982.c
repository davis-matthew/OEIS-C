/*
 * A020982  Expansion of 1/((1-9*x)*(1-10*x)*(1-11*x)).
 * https://oeis.org/A020982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11^(n+2)/2 + 9^(n+2)/2 - 10^(n+2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    idx_t e = n + 2;                     /* exponent n+2 */
    seq_t pow9  = pow_uint(9,  e);       /* 9^(n+2)   */
    seq_t pow11 = pow_uint(11, e);       /* 11^(n+2)  */
    seq_t sum   = pow9 + pow11;          /* 9^(n+2)+11^(n+2) */
    seq_t half  = sum / 2;               /* divide by 2 */
    seq_t pow10 = pow_uint(10, e);       /* 10^(n+2)  */
    return half - pow10;                 /* final value  */
}
