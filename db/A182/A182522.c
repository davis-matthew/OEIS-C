/*
 * A182522  a(0) = 1; thereafter a(2*n + 1) = 3^n, a(2*n + 2) = 2 * 3^n.
 * https://oeis.org/A182522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(2*k + 1) = 3^k a(2*k + 2) = 2 * 3^k for k ≥ 0
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 41, u64 n <= 81, u128 n <= 162,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n & 1)            /* odd index: 2*k+1 */
        return pow3(n / 2);

    /* even index: 2*k+2 */
    return pow3((n / 2) - 1) * (seq_t)2;
}
