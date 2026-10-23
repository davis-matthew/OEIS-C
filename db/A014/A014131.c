/*
 * A014131  a(n) = 2*a(n-1) if n odd else 2*a(n-1) + 6.
 * https://oeis.org/A014131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+2) - 3 - (-1)^n
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^(n+2) */
    seq_t value = pow2(n + 2);

    /* Subtract the constant 3 */
    value -= 3;

    /* Adjust for (-1)^n: even n -> subtract 1, odd n -> add 1 */
    if ((n & 1) == 0)
        value -= 1;   /* a(n) = 2^(n+2) - 4  when n even */
    else
        value += 1;   /* a(n) = 2^(n+2) - 2  when n odd */

    return value;
}
