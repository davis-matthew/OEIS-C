/*
 * A155648  a(n) = 10^n + 6^n - 1.
 * https://oeis.org/A155648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n + 6^n - 1; Offset: 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow10, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute 10^n, 6^n and sum them, then subtract 1 */
    seq_t p10 = pow10(n);           /* 10^n */
    seq_t p6  = pow_uint(6, n);     /* 6^n */
    return p10 + p6 - 1;            /* a(n) */
}
