/*
 * A052663  E.g.f. x^4*(1+x-x^2)/(1-x).
 * https://oeis.org/A052663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n < 4; a(4) = 4!; a(5) = 2·5!; a(n) = n! for n ≥ 6
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;

    seq_t r = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        r *= (seq_t)i;

    if (n == 5)
        r *= (seq_t)2;

    return r;
}
