/*
 * A086603  a(n) = n^3*3^(n-1).
 * https://oeis.org/A086603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^3 * 3^(n-1) (offset 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 31, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0)
        return 0;

    /* n^3 */
    seq_t n_term = (seq_t)n;
    seq_t n3 = n_term * n_term * n_term;

    /* 3^(n-1) computed iteratively to avoid external helper */
    seq_t pow3 = 1;
    for (idx_t i = 1; i < n; ++i)
        pow3 *= (seq_t)3;

    /* product */
    return n3 * pow3;
}
