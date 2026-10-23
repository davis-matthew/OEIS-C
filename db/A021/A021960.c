/*
 * A021960  Decimal expansion of 1/956.
 * https://oeis.org/A021960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th digit of the decimal expansion of 1/956 *
 *            (a(0)=0, a(1)=0, a(2)=1, a(3)=0, a(4)=4, a(5)=6, a(6)=0, a(7)=2,
 *            ...)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    static const seq_t cycle[7] = {1, 0, 4, 6, 0, 2, 5};
    return cycle[(size_t)((n - 2) % 7)];
}
