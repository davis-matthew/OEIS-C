/*
 * A021045  Decimal expansion of 1/41.
 * https://oeis.org/A021045
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

/* Periodic digits of 1/41: a(n) = [0,2,4,3,9] repeated every 5 */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t digits[5] = {0, 2, 4, 3, 9};
    return digits[(size_t)(n % 5)];
}
