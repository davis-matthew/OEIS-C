/*
 * A169830  Numbers k such that 2*reverse(k) - k = 1.
 * https://oeis.org/A169830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*10^(n-1) - 7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Assumes offset 1: a(1) is the first term. */
    if (n == 0)
        return (seq_t)0;          /* behavior below the offset doesn't matter */

    seq_t val = pow10(n - 1);     /* 10^(n-1) */
    val *= (seq_t)8;              /* 8*10^(n-1) */
    val -= (seq_t)7;              /* -7 */
    return val;
}
