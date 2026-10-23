/*
 * A021147  Decimal expansion of 1/143.
 * https://oeis.org/A021147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = a(n-1) - a(n-3) + a(n-4) with a(0)=0, a(1)=0, a(2)=6,
 *            a(3)=9
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t cycle[6] = {0, 0, 6, 9, 9, 3};
    return cycle[(size_t)(n % 6)];
}
