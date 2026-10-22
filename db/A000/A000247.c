/*
 * A000247  a(n) = 2^n - n - 2.
 * https://oeis.org/A000247
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^n - n - 2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base = 2;
    idx_t e = n;
    while (e > 0) {
        if (e & 1)
            result *= base;
        e >>= 1;
        if (e > 0)
            base *= base;
    }
    return result - (seq_t)n - 2;
}
