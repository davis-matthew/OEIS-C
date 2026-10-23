/*
 * A164034  a(n) = ((4+3*sqrt(2))*(4+sqrt(2))^n +
 *          (4-3*sqrt(2))*(4-sqrt(2))^n)/4.
 * https://oeis.org/A164034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 14*a(n-2), a(0)=2, a(1)=11
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)11;

    seq_t a0 = (seq_t)2;
    seq_t a1 = (seq_t)11;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t a2 = a1 * (seq_t)8 - a0 * (seq_t)14;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
