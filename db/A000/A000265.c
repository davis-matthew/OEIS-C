/*
 * A000265  Remove all factors of 2 from n; or largest odd divisor of n; or
 *          odd part of n.
 * https://oeis.org/A000265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy, nice
 * Formula:   a(n) = if n is odd then n, otherwise a(n/2); i.e. a(n) =
 *            n/A006519(n)
 * Range:     u8 n <= 256, u16 n <= 8274, u32 n <= 8274, u64 n <= 8274,
 *            u128 n <= 8274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    while ((n & 1) == 0)
        n >>= 1;
    return (seq_t)n;
}
