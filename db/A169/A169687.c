/*
 * A169687  a(n) = 3^n - 3*2^(n-2).
 * https://oeis.org/A169687
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 3*2^(n-2) (offset n=2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* values below the offset are irrelevant */

    /* 3^n */
    seq_t p3 = pow_uint(3, n);

    /* 2^(n-2) */
    seq_t p2 = pow_uint(2, n - 2);

    /* a(n) = 3^n - 3*2^(n-2) */
    return p3 - (seq_t)3 * p2;
}
