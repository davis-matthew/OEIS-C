/*
 * A122970  29th powers: a(n) = n^29.
 * https://oeis.org/A122970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = n^29
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    seq_t base = (seq_t)n;
    for (idx_t i = 0; i < 29; ++i)
        res *= base;
    return res;
}
