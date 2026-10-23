/*
 * A021032  Decimal expansion of 1/28.
 * https://oeis.org/A021032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A021032: Decimal digits of 1/28.
   a(0) = 0, a(1) = 3; for n >= 2, a(n) = cycle[(n-2) mod 6] where
   cycle = {5, 7, 1, 4, 2, 8} . */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;
    if (n == 1u)
        return 3;
    static const seq_t cycle[6] = {5, 7, 1, 4, 2, 8};
    return cycle[(size_t)((n - 2u) % 6u)];
}
