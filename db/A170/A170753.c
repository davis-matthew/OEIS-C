/*
 * A170753  Expansion of g.f.: (1+x)/(1-33*x).
 * https://oeis.org/A170753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*33^(n-1) for n>0, a(0)=1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0u)
        return 1;
    seq_t result = 34;
    for (idx_t i = 1; i < n; ++i)
        result *= 33;
    return result;
}
