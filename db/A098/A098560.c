/*
 * A098560  Expansion of e.g.f. (1+4*x)/(1-4*x).
 * https://oeis.org/A098560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*4^n*n! - 0^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i)
        result *= (seq_t)4 * (seq_t)i;   /* multiply by 4*i each step */

    result *= (seq_t)2;                   /* final factor of 2 */
    return result;
}
