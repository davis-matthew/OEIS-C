/*
 * A152556  a(n) = 2*(2*n+2)^floor((n-1)/2).
 * https://oeis.org/A152556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (2*n + 2) ^ floor((n-1)/2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 40,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t base = (seq_t)(2 * n + 2);
    idx_t  exp  = (n - 1) >> 1;          /* floor((n-1)/2) */
    seq_t  pow  = big_pow(base, (seq_t)exp);

    return pow * (seq_t)2;
}
