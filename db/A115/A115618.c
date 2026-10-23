/*
 * A115618  a(n) = 1 + (n+6)*2^(n-1).
 * https://oeis.org/A115618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (n+6)*2^(n-1)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)4;          /* 1 + 6 * (1/2) = 4 */
    }
    /* For n >= 1, compute 1 + (n+6) * 2^(n-1) */
    seq_t exponent = pow2(n - 1);          /* 2^(n-1) */
    seq_t coeff    = (seq_t)(n + 6);       /* n + 6 */
    return coeff * exponent + (seq_t)1;
}
