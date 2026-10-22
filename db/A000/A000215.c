/*
 * A000215  Fermat numbers: a(n) = 2^(2^n) + 1.
 * https://oeis.org/A000215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (a(n-1)-1)^2 + 1, a(0) = 3; i.e. a(n) = 2^(2^n) + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = 2;
    idx_t i;
    for (i = 0; i < n; i++) {
        x = x * x;
        if (x == 0)
            break; /* fixed-width overflow: value no longer representable */
    }
    return x + 1;
}
