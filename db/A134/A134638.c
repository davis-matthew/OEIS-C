/*
 * A134638  Row sums of triangle A134637.
 * https://oeis.org/A134638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -8 + 15*2^(n-1) - 5*n - n^2 Offset: 1
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 985 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    /* n is expected to be >= 1; calls with n == 0 return 0 */
    if (n == 0) return 0;

    seq_t pow2 = ipow_i32((seq_t)2, n - 1);   /* 2^(n-1) */
    seq_t sn   = (seq_t)n;                   /* cast n to seq_t */

    return (seq_t)15 * pow2 - (seq_t)5 * sn - sn * sn - (seq_t)8;
}
