/*
 * A021433  Decimal expansion of 1/429.
 * https://oeis.org/A021433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = {0,0,2,3,3,1}[n mod 6]
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021433: decimal expansion of 1/429. */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[6] = {0, 0, 2, 3, 3, 1};
    return period[(size_t)(n % 6)];
}
