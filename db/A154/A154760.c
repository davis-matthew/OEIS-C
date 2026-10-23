/*
 * A154760  Final digit of n!! (A006882).
 * https://oeis.org/A154760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=8, a(5)=5, * a(6)=8,
 *            a(7)=5, a(8)=4; for n≥9, a(n)=0 if n is even else 5
 * Range:     u8 n <= 118, u16 n <= 118, u32 n <= 118, u64 n <= 118,
 *            u128 n <= 118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 8) {
        static const seq_t vals[] = {1, 1, 2, 3, 8, 5, 8, 5, 4};
        return vals[(size_t)n];
    }
    return (n & 1) ? 5 : 0;
}
