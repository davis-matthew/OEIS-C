/*
 * A021574  Decimal expansion of 1/570.
 * https://oeis.org/A021574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 0 for n < 2 * a(n) = cycle[(n-2) mod 18] for n >= 2, *
 *            where cycle = {1,7,5,4,3,8,5,9,6,4,9,1,2,2,8,0,7,0} (the
 *            repeating part of 1/570)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    static const seq_t cycle[18] = {
        1, 7, 5, 4, 3, 8, 5, 9, 6, 4,
        9, 1, 2, 2, 8, 0, 7, 0
    };

    return cycle[(size_t)((n - 2) % 18)];
}
