/*
 * A171389  a(n) = 21*2^n - 1.
 * https://oeis.org/A171389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*2^n - 1
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 951 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 21 * 2^n - 1 */
    seq_t two_pow = pow2(n);          /* 2^n as seq_t */
    seq_t result  = two_pow * 21;     /* 21 * 2^n */
    result -= (seq_t)1;              /* subtract 1 */
    return result;
}
