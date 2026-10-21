/*
 * A074528  a(n) = 2^n + 3^n + 6^n.
 * https://oeis.org/A074528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 3^n + 6^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t two   = pow2(n);      /* 2^n */
    seq_t three = pow3(n);      /* 3^n */
    seq_t six   = two * three;  /* 6^n = 2^n * 3^n */
    return two + three + six;   /* a(n) = 2^n + 3^n + 6^n */
}
