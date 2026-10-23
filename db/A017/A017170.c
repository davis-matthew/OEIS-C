/*
 * A017170  a(n) = (9*n)^10.
 * https://oeis.org/A017170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (9*n)^10
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 9, u128 n <= 792,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t base = (seq_t)9 * (seq_t)n;
    seq_t result = base;
    for (idx_t i = 1; i < 10; ++i)
        result *= base;
    return result;
}
