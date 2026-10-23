/*
 * A014113  a(n) = a(n-1) + 2*a(n-2) with a(0)=0, a(1)=2.
 * https://oeis.org/A014113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) with a(0)=0, a(1)=2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    seq_t a0 = 0;
    seq_t a1 = 2;
    if (n == 0)
        return a0;
    if (n == 1)
        return a1;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t temp = a1 + a0;
        temp = temp + a0;    /* temp = a1 + 2*a0 */
        a0 = a1;
        a1 = temp;
    }
    return a1;
}
