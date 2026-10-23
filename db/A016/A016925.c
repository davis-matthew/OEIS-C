/*
 * A016925  a(n) = (6*n + 1)^5.
 * https://oeis.org/A016925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 1)^5
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 13, u64 n <= 1188,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 6 + 1;
    seq_t result = base;
    for (int i = 1; i < 5; ++i)
        result *= base;
    return result;
}
