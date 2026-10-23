/*
 * A021023  Decimal expansion of 1/19.
 * https://oeis.org/A021023
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

/* Sequence: digits of 1/19 (period 18) */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t cycle[18] = {
        0, 5, 2, 6, 3, 1, 5, 7,
        8, 9, 4, 7, 3, 6, 8, 4,
        2, 1
    };
    return cycle[(size_t)(n % 18)];
}
