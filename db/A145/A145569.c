/*
 * A145569  Multiples of 6 appear in pairs.
 * https://oeis.org/A145569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - floor(n/7) (for n >= 0, a(0)=0)
 * Range:     u8 n <= 297, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n = 0 the formula gives 0 – matches the OEIS sequence. */
    return (seq_t)n - (seq_t)(n / 7);
}
