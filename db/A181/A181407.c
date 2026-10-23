/*
 * A181407  a(n) = (n-4)*(n-3)*2^(n-2).
 * https://oeis.org/A181407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-4)*(n-3)*2^(n-2)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 25, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1)                     /* n = 0 or 1 */
        return (seq_t)3;
    if (n == 2)                     /* n = 2 */
        return (seq_t)2;
    if (n <= 4)                     /* n = 3 or 4 */
        return (seq_t)0;

    /* n >= 5: use the direct formula */
    seq_t coeff = (seq_t)(n - 4) * (seq_t)(n - 3);
    seq_t pw    = pow2(n - 2);      /* 2^(n-2) */
    return coeff * pw;
}
