/*
 * A045623  Number of 1's in all compositions of n+1.
 * https://oeis.org/A045623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 2, a(n) = (n+3) * 2^(n-2) for n >= 2
 *            (equivalently a(n+1) = 2*a(n) + 2^(n-1) for n > 0)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp) */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    /* n >= 2 */
    seq_t power = pow2(n - 2);       /* 2^(n-2) */
    seq_t coeff = (seq_t)(n + 3);    /* n+3 */
    return coeff * power;
}
