/*
 * A060919  Number of corners in a 4-sided fractal.
 * https://oeis.org/A060919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n+2)*(2^n+4)/6 (OEIS A060919, offset 1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(1) = 4, offset is 1 */
    if (n == 0) return 0;          /* unused but harmless */
    /* Compute 2^(n-1) */
    seq_t b = pow2(n - 1);         /* 2^(n-1) */
    /* Build the two factors of the numerator */
    seq_t c = (b << 1) + 2;        /* 2*2^(n-1) + 2  = 2^n + 2 */
    seq_t d = (b << 1) + 4;        /* 2*2^(n-1) + 4  = 2^n + 4 */

    /* One of c or d is divisible by 6 depending on parity of n */
    if (n & 1)                     /* n is odd → d divisible by 6 */
        return c * (d / 6);
    else                           /* n is even → c divisible by 6 */
        return (c / 6) * d;
}
