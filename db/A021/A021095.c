/*
 * A021095  Decimal expansion of 1/91.
 * https://oeis.org/A021095
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

/* Pattern: a(n) = [0,1,0,9,8,9][n mod 6] */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pat[6] = {0, 1, 0, 9, 8, 9};
    return pat[(size_t)(n % 6)];
}
