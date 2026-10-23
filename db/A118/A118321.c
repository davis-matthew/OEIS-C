/*
 * A118321  Decimal expansion of 8/105.
 * https://oeis.org/A118321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy, changed
 * Formula:   a(0)=0; for n>=1, a(n) = {7,6,1,9,0,4}[(n-1)%6] digits of the
 *            decimal expansion of 8/105
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    static const seq_t pat[6] = {7,6,1,9,0,4};
    return pat[(size_t)((n - 1) % 6)];
}
