/*
 * A021067  Decimal expansion of 1/63.
 * https://oeis.org/A021067
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

/*  Periodic decimal digits of 1/63.  a(n) = {0,1,5,8,7,3} repeating every 6 positions. */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t v[6] = {0, 1, 5, 8, 7, 3};
    return v[(size_t)(n % 6)];
}
