/*
 * A181355  a(3*n+1) = 4^(2^n), a(3*n+2) = 3^(2^n), a(3*n+3) = 4^(2^n) -
 *          3^(2^n).
 * https://oeis.org/A181355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(3*n+1)=4^(2^n), a(3*n+2)=3^(2^n), a(3*n+3)=4^(2^n)-3^(2^n)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 12, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   pow_two_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_two_exp.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1 */
    if (n == 0)
        return (seq_t)0;

    /* Determine block number and position within the block */
    idx_t m = (n - 1) / 3;          /* block index */
    idx_t k = (n - 1) % 3;          /* 0 -> first, 1 -> second, 2 -> third term */

    /* 4^(2^m) */
    seq_t t4 = pow_two_exp((seq_t)4, m);

    if (k == 0)                /* a(3*m+1) */
        return t4;

    /* 3^(2^m) */
    seq_t t3 = pow_two_exp((seq_t)3, m);

    if (k == 1)                /* a(3*m+2) */
        return t3;

    /* a(3*m+3) = 4^(2^m) - 3^(2^m) */
    return t4 - t3;
}
